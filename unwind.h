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

#ifndef UNWIND_H
#define UNWIND_H

class TinyInst;
class ModuleInfo;

class UnwindData {
  virtual ~UnwindData() = default;
};

class UnwindGenerator {
public:
  UnwindGenerator(TinyInst& tinyinst) : tinyinst_(tinyinst) {}
  virtual ~UnwindGenerator() = default;

  void OnModuleInstrumented(ModuleInfo* module) { }
  void OnModuleUninstrumented(ModuleInfo* module) { }

  size_t MaybeRedirectExecution(ModuleInfo* module, size_t IP) {
    return IP;
  }

  void OnBasicBlockStart(ModuleInfo* module,
                         size_t original_address,
                         size_t translated_address)
  { }

  void OnInstruction(ModuleInfo* module,
                     size_t original_address,
                     size_t translated_address)
  { }

  void OnBasicBlockEnd(ModuleInfo* module,
                       size_t original_address,
                       size_t translated_address)
  { }

protected:
	TinyInst& tinyinst_;
};

#endif // UNWIND_H