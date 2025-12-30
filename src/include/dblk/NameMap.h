

#ifndef MYEXTENSION_NAMEMAP_H
#define MYEXTENSION_NAMEMAP_H
#include "unordered_map"
#include "vector"
#include "string_view"
#include "StringTableAllocator.h"
#include "vartypes.h"
/**
 * A NameMap is a class which allows for fast lookup
 */
class NameMap {
public:

  NameMap& operator=(const NameMap&) = delete;


  /// returns a name given a nameId
  inline const char * getName(const name_id name);
  inline const char * getName(const name_id name) const;
  /// returns a name_id given a name
  inline name_id getNameId(std::string_view name);
  inline name_id getNameId(const char * name) {return getNameId(std::string_view(name));}
  inline name_id getNameId(std::string_view name) const;
  inline const name_id getNameId(const char * name) const {return getNameId(std::string_view(name));}

  /// adds a name to the name map and returns its name_id, if the name exists, returns the existing name_id instead
  inline name_id addName(std::string_view name);
  inline name_id addName(const char * name) {return addName(std::string_view(name));}
  inline name_id addName(std::string_view name) const;
  inline name_id addName(const char * name) const {return addName(std::string_view(name));}
protected:
  std::unordered_map<std::string_view, name_id> lookup; // lookup a name_id from a name
  std::vector<name_id> name_id_to_offset; //  relates a name_Id to an index in names
  StringTableAllocator names; // stores all the strings
};


#endif //MYEXTENSION_NAMEMAP_H
