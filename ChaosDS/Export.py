import uno 


def HelloWorldPython(arg):
      """Prints the string 'Hello World(in Python)' into the current document"""
      #get the doc from the scripting context which is made available to all scripts
      try:
        model = XSCRIPTCONTEXT.getDocument()
        
        spreadsheets = model.getSheets()
        dataSpreadsheet = spreadsheets.getByName("Data") 
        oCel1 = dataSpreadsheet.getCellByPosition(0, 0)
        print oCel1.getFormula()
      except Exception,e:
        print "Exception:"+str(e)
