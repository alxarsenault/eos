//
//  axOSTerminal.h
//  Minimal
//
//  Created by Alexandre Arsenault on 2015-08-30.
//  Copyright (c) 2015 axLib. All rights reserved.
//

#ifndef __Minimal__axOSTerminal__
#define __Minimal__axOSTerminal__

#include "axLib/axLib.h"

#include <thread>
#include <algorithm>

namespace ax
{
    namespace os
    {
        class Terminal : public ax::Event::Object
        {
        public:
            enum : ax::Event::Id
            {
                READ_CHILD_BEGIN,
                READ_CHILD_OUTPUT,
                READ_CHILD_END
            };

            Terminal(ax::Event::Manager* manager);
            
            bool Execute(const std::string& program_folder_path,
                         const std::string& program_name,
                         const ax::StringVector& args);
            
            bool Write(const std::string& command);
            
        private:
            std::thread _read_child_thread;
            int _fd_master;
            
            static void ReadFromChild(Terminal& terminal, const int& stdin_fd);
        };
    }
}

#endif /* defined(__Minimal__axOSTerminal__) */
