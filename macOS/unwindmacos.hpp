/*
Copyright 2021 Google LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

https ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef unwindmacos_hpp
#define unwindmacos_hpp

#include <vector>

#include <stdio.h>
#include "unwind.h"
#include "tinyinst.h"
#include <mach-o/compact_unwind_encoding.h>

class UnwindDataMacOS: public UnwindData {
public:
  UnwindDataMacOS();
  ~UnwindDataMacOS() = default;

  void *addr;
  uint64_t size;
  void *buffer;
  struct unwind_info_section_header *header;

  void AddEncoding(compact_unwind_encoding_t encoding,
                   size_t translated_address);

  struct Metadata {
    compact_unwind_encoding_t encoding;
    size_t min_address;
    size_t max_address;

    Metadata();

    Metadata(compact_unwind_encoding_t encoding,
             size_t min_address,
             size_t max_address)
    : encoding(encoding),
      min_address(min_address),
      max_address(max_address)
    {}
  };

  std::vector<Metadata> metadata_list;
};

class UnwindGeneratorMacOS : public UnwindGenerator {
public:
  UnwindGeneratorMacOS(TinyInst& tinyinst);
  ~UnwindGeneratorMacOS() = default;

  void OnModuleInstrumented(ModuleInfo* module);
  void OnModuleUninstrumented(ModuleInfo* module);

//  size_t MaybeRedirectExecution(ModuleInfo* module, size_t IP) {
//    return IP;
//  }
//
  void OnBasicBlockStart(ModuleInfo* module,
                         size_t original_address,
                         size_t translated_address);

  void OnInstruction(ModuleInfo* module,
                     size_t original_address,
                     size_t translated_address);

  void OnBasicBlockEnd(ModuleInfo* module,
                       size_t original_address,
                       size_t translated_address);

private:
  void FirstLevelLookup(ModuleInfo *module, size_t original_address, size_t translated_address);
  void SecondLevelLookup(ModuleInfo *module,
                        size_t original_address,
                        size_t translated_address,
                        struct unwind_info_section_header_index_entry *first_level_entry);
  void SecondLevelLookupCompressed(ModuleInfo *module,
                                   size_t original_address,
                                   size_t translated_address,
                                   struct unwind_info_section_header_index_entry *first_level_entry,
                                   size_t second_level_page_addr);
  void SecondLevelLookupRegular(ModuleInfo *module,
                                   size_t original_address,
                                   size_t translated_address,
                                   struct unwind_info_section_header_index_entry *first_level_entry,
                                   size_t second_level_page_addr);
};

#endif /* unwindmacos_hpp */
