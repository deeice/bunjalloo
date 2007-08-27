#ifndef TextAreaFactory_h_seen
#define TextAreaFactory_h_seen

class TextArea;
class TextAreaFactory
{

  public:
    /** The user is responsible for calling delete on each TextArea created.
     * @return a new TextArea.
     */
    static TextArea * create();

  private:
    TextAreaFactory();
    ~TextAreaFactory();
    TextAreaFactory(const TextAreaFactory &);
    const TextAreaFactory operator=(const TextAreaFactory &);

};
#endif
