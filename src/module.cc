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
 * @author "Maximilian Jung <maxjung96@gmx.de>"
 * @author "Oliver Schranz <oliver.schranz@cispa.saarland>"
 *
 */

#include <artist/api/filtering/method_name_filters.h>

#include "module.h"
#include "pass.h"
#include "codelib.h"

using std::make_shared;
using std::unique_ptr;

using art::ModuleId;

using namespace art;

GunshopModule::GunshopModule(const shared_ptr<const art::FilesystemHelper> fs) : Module(fs) {}

HArtist * GunshopModule::createPass(const MethodInfo &method_info) const {
  return new (method_info.GetGraph()->GetArena()) HGunshopArtist(method_info);
}

shared_ptr<const CodeLib> GunshopModule::createCodeLib() const {
    return make_shared<GunshopCodeLib>();
}


unique_ptr<Filter> GunshopModule::getMethodFilter() const {
    // we only want to apply the instrumentation passes to methods in the gunshop package
  const vector<const string> ui = {"gunshop."};
  return unique_ptr<Filter>(new MethodNameWhitelist(ui, false, true));
}

extern "C" shared_ptr<Module> create(shared_ptr<const FilesystemHelper> fshelper) {
    return make_shared<GunshopModule>(fshelper);
}

extern "C" ModuleId get_id() {
  return "de.alfink.ctf.asisfinals2018.gunshop.proxy";
}
