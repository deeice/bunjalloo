#ifndef Cookie_h_seen
#define Cookie_h_seen

class Cookie
{

  public:
    Cookie()
    {}

    Cookie(const std::string & name,
           const std::string & value,
           int                 port,
           const std::string & domain,
           bool                domainInitialDot,
           const std::string & path
        )
      : m_name(name),
        m_value(value),
        m_port(port),
        m_domain(domain)
        m_domainInitialDot(domainInitialDot),
        m_path(path)
    {}

  private:
    std::string m_name;
    std::string m_value;
    int         m_port;
    std::string m_domain;
    bool        m_domainInitialDot;
    std::string m_path;

};
#endif
