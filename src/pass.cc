/**
 * The ARTist Project (https://artist.cispa.saarland)
 *
 * Copyright (C) 2018 CISPA (https://cispa.saarland), Saarland University
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author "Alexander Fink <alexander.fink@cispa.saarland>"
 *
 */

#include <atomic>

#include <artist/api/io/artist_log.h>
#include <artist/api/io/error_handler.h>

#include "pass.h"
#include "codelib.h"

using std::string;
using std::vector;
using std::find_if;

using art::INFO;
using art::HIntConstant;
using art::HParameterValue;
using art::ErrorHandler;

using art::ArtUtils;
using art::Primitive;

void HGunshopArtist::RunPass() {
    // Initializing some variables
    auto allocator = graph_->GetArena();
    auto env = getCodeLibEnvironment();
    const art::DexFile& current = graph_->GetDexFile();
    auto symbols = env->getCodelibSymbols(&current);
    // art::ArtistLog::LogMethod(_method_info);
    for (auto block: graph_->GetBlocks()){
        /*
         * Iterate over all basic blocks of the method
         */
        if (block != nullptr) {
            auto instr = block->GetFirstInstruction();
            while (instr != nullptr){
                /*
                 * Iterate over all instructions in the basic block
                 */
                if (instr->IsInvoke()) {
                    auto ins = instr->AsInvoke(); // **ins** contains the current instruction cursor
                    if (ArtUtils::GetMethodName(ins, true).find("android.gunshop.com.gunshop.m.a(android.content.Context, java.lang.String)") != string::npos) {
                        /*
                         * If the invoke instruction calls the key-generation method:
                         * we add a call to a CodeLib method that logs the key to logcat afterwards
                         */
                        // Create an instance of the codelib class:
                        auto codelib_instruction = GetCodeLibInstruction();
                        // The parameters is the codelib instance and the return value of **ins** (the encryption key):
                        vector<art::HInstruction *> params = {codelib_instruction, ins};
                        // signature of our codelib method:
                        string invoked_signature = GunshopCodeLib::LOG;
                        // Create an invoke to our codelib method:
                        auto invoke_codelib_method = new (allocator) art::HInvokeVirtual(allocator,
                                                                               static_cast<uint32_t>(params.size()), art::Primitive::kPrimVoid, 0,
                                                                               symbols->getMethodIdx(
                                                                                       invoked_signature),
                                                                               (uint32_t) env->getMethodVtableIdx(
                                                                                       invoked_signature));
                        // Set the params:
                        ArtUtils::SetupInstructionArguments(invoke_codelib_method, params);
                        // Inserts the instruction after the one invoking the key generation method
                        block->InsertInstructionAfter(invoke_codelib_method, ins);
                    }
                }
                if (instr->IsReturn()){
                    // All return instructions
                    auto ins = instr->AsReturn();
                    if (_method_info.GetMethodName(true).find("boolean android.gunshop.com.gunshop.m.a(javax.net.ssl.HttpsURLConnection, java.util.Set)") != string::npos){
                        /*
                         * To disable the manual certificate pinning, the value of every return statement in this method is set to true
                         */
                        // create a new boolean variable:
                        auto replacement = graph_->GetConstant(art::Primitive::kPrimBoolean, 1,0);
                        // set the new variable used for the return value:
                        ins->SetRawInputAt(0,replacement);
                    }
                }
                if (instr->IsInvokeVirtual()){
                    auto ins = instr->AsInvokeVirtual();
                    if (ArtUtils::GetMethodName(ins) == "java.net.URL.openConnection") {
                        /*
                         * Every invokation of java.net.URL.openConnection is replaced with an invokation of
                         * our CodeLib.openConnection that sets up the proxy and disables certificate checks.
                         *
                         * Works similar to the first example
                         */
                        auto codelib_instruction = GetCodeLibInstruction();
                        // Now we want the input to be the input of the original openConnection()-Invoke
                        vector<art::HInstruction *> params = {codelib_instruction, ins->InputAt(0)};
                        string invoked_signature = GunshopCodeLib::OPENCONNECTION;
                        auto replacement = new (allocator) art::HInvokeVirtual(allocator,
                                                                               static_cast<uint32_t>(params.size()), art::Primitive::kPrimNot, 0,
                                                                               symbols->getMethodIdx(
                                                                                                invoked_signature),
                                                                               (uint32_t) env->getMethodVtableIdx(
                                                                                                invoked_signature));
                        ArtUtils::SetupInstructionArguments(replacement, params);
                        // We do not keep the openConnection()-Invoke, but replace it with our own.
                        block->ReplaceAndRemoveInstructionWith(ins, replacement);
                    }
                }
                if (instr == block->GetLastInstruction()){
                    break;
                }
                instr = instr->GetNext();
            }
        }
    }

}
