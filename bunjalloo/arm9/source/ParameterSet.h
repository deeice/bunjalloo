#ifndef ParameterSet_h_seen
#define ParameterSet_h_seen

#include <map>
#include <string>

typedef std::map<std::string, std::string> KeyValueMap;
class PSImpl;
class ParameterSet
{
  public:

    /** Create a parameter set based on a semi-colon (;) seperated list of key, value pairs.
     * @param semiColonSeperatedKeyValueString the semi-colon seperated list of key, value pairs.
     */
    ParameterSet(const std::string & semiColonSeperatedKeyValueString);

    /** Check if this set has the requested parameter.
     * @param key the name of the parameter to check.
     * @return true if the parameter is in the set.
     */
    bool hasParameter(const std::string & key) const;
    /** Fetch a parameter value. Convenience function to save having to retrieve and traverse the map each time.
     * @param key the parameter name.
     * @param value the value is returned in this para.
     * @return true if the parameter is set, false otherwise.
     */
    bool parameter(const std::string & key, std::string & value) const;
    /** Fetch a map of paramter - value pairs.
     * @return a standard map of key,value pairs. The keys are the parameter names.
     */
    const KeyValueMap & keyValueMap() const;

  private:
    PSImpl * m_psImpl;

};

#endif
