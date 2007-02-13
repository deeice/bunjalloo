import uno 
import sys
import os

class FolderPicker:
  def __init__(self):
    ctx = uno.getComponentContext()
    smgr = ctx.ServiceManager
    folderPicker = smgr.createInstanceWithContext( 
                    'com.sun.star.ui.dialogs.FolderPicker', ctx)
    if os.environ.has_key('HOME'):
      folderPicker.setDisplayDirectory(os.environ['HOME'])
    else:
      # cwd is shot to pieces
      folderPicker.setDisplayDirectory(os.getcwd())
    result = folderPicker.execute()
    if result:
      self.directory = uno.pyuno.fileUrlToSystemPath(folderPicker.getDirectory())
    else:
      self.directory = ''


class SpellData:
  def __init__(self, row0, row):
    ## row 0 defines stuff
    self.map = {
      'spellName':'',
      'chaosRating':0,
      'castChance':0,
      'castRange':0,
      'castPriority':0,
      'palette':None,
      'spellFunction':0,
      'combat':0,
      'rangedCombat':0,
      'rangedCombatRange':0,
      'defence':0,
      'movement':0,
      'manvr':0,
      'magicRes':0,
      'gfxData':0,
      'mapData':0
      
    }
    self.row0 = row0
    for i in range(len(row)):
      if row[i] == '':
        continue
      value = None
      try:
        value = int(row[i])
      except:
        value = row[i]
      self.map[row0[i]] = value
    if self.map['palette'] != None:
      self.map['gfxData'] = self.nameToData('raw')
      self.map['mapData'] = self.nameToData('map')
    else:
      self.map['palette'] = 0
      
  def nameToData(self,dataType):
    name = self.map['spellName']
    # convert spaces to _, lowercase and add _binary_<name>_map_start
    name = name.replace(' ','_').lower()
    return '_binary_'+name+'_'+dataType+'_start'

  def __str__(self):
    returnString = '{'
    for key in self.row0:
      isString =  key=='spellName'
      if isString: returnString += '"'
      returnString += str(self.map[key])
      if isString: returnString += '"'
      returnString += ', '
    returnString += '},'
    return returnString
    
def getUsedCells(sheet):
  row_ind = 0
  col_ind = 0
  cells = []
  cell = sheet.getCellByPosition(col_ind,row_ind)
  maxLength = 0
  while cell.getFormula() != '':
    # go along row
    row = []
    while cell.getFormula() != '' or col_ind < maxLength:
      col_ind += 1
      row.append(cell.getFormula())
      cell = sheet.getCellByPosition(col_ind,row_ind)
    if col_ind > maxLength:
      maxLength = col_ind
    cells.append(row)
    col_ind = 0
    row_ind += 1
    cell = sheet.getCellByPosition(col_ind,row_ind)
  return cells  

def writeHeader(headerFile, row0, types):
  
  headerFile.write('#ifndef SpellData_h_seen\n#define SpellData_h_seen\n')
  headerFile.write('typedef void (*FunctionPtr_t)(void);\n')
  headerFile.write('typedef struct  {\n')
  for i in range(len(types)):
    headerFile.write('  '+types[i]+' '+row0[i]+';\n')
  headerFile.write('} SpellData_t;\n')
  headerFile.write('extern const SpellData_t s_spellData[];\n')
  headerFile.write('#endif\n')

def writeBody(bodyFile, spells):
  bodyFile.write('#include <nds/jtypes.h>\n')
  bodyFile.write('#include "SpellData.h"\n')
  bodyFile.write('#include "images.h"\n')
  bodyFile.write('#include "magic.h"\n')
  bodyFile.write('const SpellData_t s_spellData[] = {\n')
  for i in spells:
    bodyFile.write('  '+str(i)+'\n')
  bodyFile.write('};\n')


def ExportData(args=None):
  """Exports data from document to struct format"""
  #get the doc from the scripting context which is made available to all scripts
  try:
    model = XSCRIPTCONTEXT.getDocument()
    # create the dialog model and set the properties 
    spreadsheets = model.getSheets()
    dataSpreadsheet = spreadsheets.getByName('Data')
    cells = getUsedCells(dataSpreadsheet)
    typeRow = cells[0]
    typeRow.append('const u16 *')
    typeRow.append('const u16 *')
    row0 = cells[1]
    row0.append('gfxData')
    row0.append('mapData')
    spells = []
    for row in cells[2:]:
      spells.append(SpellData(row0, row))
      
    f = FolderPicker()
    if f.directory != '':
      spellData_c = open(f.directory + '/SpellData.cpp','w')
      spellData_h = open(f.directory + '/SpellData.h','w')
      writeHeader(spellData_h, row0, typeRow)
      # write the h file
      writeBody(spellData_c, spells)
      
  except Exception,e:
    print "Exception:"+str(e)

g_exportedScripts = ExportData,
