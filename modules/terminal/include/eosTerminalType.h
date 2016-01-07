#ifndef __EOS_TERMINAL_TYPE_H__
#define __EOS_TERMINAL_TYPE_H__

#include <axLib/axLib.h>
#include <map>

namespace eos {
typedef std::pair<unsigned int, unsigned int> CharIndex;
typedef std::map<CharIndex, ax::Color> ColorMap;
typedef std::map<CharIndex, bool> BoldMap;
}

#endif // __EOS_TERMINAL_TYPE_H__