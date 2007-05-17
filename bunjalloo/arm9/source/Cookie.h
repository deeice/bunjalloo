#ifndef Cookie_h_seen
#define Cookie_h_seen

#include <string>

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
        m_domain(domain),
        m_domainInitialDot(domainInitialDot),
        m_path(path)
    {}

    bool matchesDomain(const std::string & domain) const
    {
      return domain == m_domain;
    }

    const std::string & name() const
    {
      return m_name;
    }

    const std::string & value() const
    {
      return m_value;
    }

  private:
    std::string m_name;
    std::string m_value;
    int         m_port;
    std::string m_domain;
    bool        m_domainInitialDot;
    std::string m_path;

};
#endif
