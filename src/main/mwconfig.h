// Micro Wave Standard Template Library
// Copyright (c) 2015-2020, Ark Lee
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// You must obey the GNU General Public License in all respects for
// all of the code used.  If you modify file(s) with this exception, 
// you may extend this exception to your version of the file(s), but 
// you are not obligated to do so.  If you do not wish to do so, delete 
// this exception statement from your version. If you delete this exception 
// statement from all source files in the program, then also delete it here.
//
// Contact:  Ark Lee <arklee@houduan.online>
//           Beijing, China


#ifndef _MW_CONFIGURATOR_H_
#define _MW_CONFIGURATOR_H_

#include <map>
#include <string.h>
#include <set>

#define LINE_LEN 1024
#define CONF_KEY_VAL_SEPER "="
#define CONF_VAL_SEPER ","


struct ConfVal {
    std::string _val;
    int _num;

  public:
    ConfVal(const std::string &val, int num)
        : _val(val)
        , _num(num) {
    }
};

class MWConfig {
  private:
    std::string _file_name;
    std::map<std::string, ConfVal> _key_vals;
    int _num;

  private:
    int Parse(char *line);
    void Clear();
    int ParseLines(FILE *conf);
    int Insert(const std::string &key, const std::string &val);

  public:
    MWConfig(const std::string &file_name);

    int Load();
    std::set<std::string> GetKeys() const;
    int GetKeysNum() const;
    int GetIntVal(const std::string &key) const;
    std::string GetStringVal(const std::string &key) const;
};

#endif
