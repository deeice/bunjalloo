#ifndef Line_h_seen
#define Line_h_seen

//! Line of sight and spell casting.
class Arena;
class Line
{
  public:


    //!Types of lines available.
    enum Line_t {
      SIGHT=0,     //!< No line is drawn.
      ARROW,       //!< A normal size arrow.
      SHORT_ARROW, //!< A short size arrow.
      SPELL,       //!< The spell cast line.
      DRAGON,      //!< Dragon breath type.
      BOLT,        //!< A single blob .
      LIGHTNING    //!< Multiple blob, looks like dragon breath type.
    };
    //! helper fn for drawing the line animation
    static bool doLine(Line_t);

    //! create a line
    Line(Line_t ls);
    //! destroy a line...
    ~Line();

    /*! @brief execute the line animation
     * @return if the line of sight was blocked (SIGHT type only)
     */
    bool execute();

  private:
    Line_t m_type;
    Arena & m_theArena;
    int m_pattern;
    int m_tableOffset;
    int m_segmentCount;
    int m_length;
    int m_gfx;
    int m_end;
    unsigned short m_colour;
    unsigned short * m_mapData;
    unsigned short * m_tileData;

    Line(const Line &);
    const Line & operator=(const Line &);

    void drawPixel4bpp(unsigned short x, unsigned short y);
    void xyUpper(int index, int & x, int & y) const;
    bool isPixelSet(unsigned int x, unsigned int y) const;
    void drawLineSegment(int x, int y);
    void innerDrawSegment(int x, int y);
    void setPixel(unsigned short x, unsigned short y);
    void removePixel(unsigned short x, unsigned short y);


    unsigned short & pixel(int x, int y) const;

};
#endif
