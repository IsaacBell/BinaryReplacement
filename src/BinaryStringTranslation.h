#include <string>
#include <map>

class BinaryStringTranslation {
private:
  typedef std::map<std::string, std::string> SwapList;
  static SwapList swap_list;
public:
  std::string translateBinary(std::string);
};
