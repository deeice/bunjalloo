#ifndef Image_h_seen
#define Image_h_seen

class Image
{
  public:
    Image(const char * filename);
    ~Image();

    bool isValid() const;

    unsigned int width() const;
    unsigned int height() const;
    unsigned int paletteSize() const;
    const unsigned short * palette() const;
    

    // RGB data.
    const unsigned char * data() const;

  private:
    bool m_valid;
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_paletteSize;
    unsigned char * m_data;
    unsigned short * m_palette;
    
    void readPng(const char *filename);

};
#endif
