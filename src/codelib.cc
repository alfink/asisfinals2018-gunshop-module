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
 */

#include "codelib.h"


// CODE_GENERATOR //////////////////////////////////////////////////////////////////////////////////////////////////////
// METHODS //////////////////////////////////
const string GunshopCodeLib::OPENCONNECTION("Lsaarland/cispa/artist/codelib/CodeLib;openConnection(Ljava/net/URL;)Ljava/net/URLConnection;");
const string GunshopCodeLib::LOG("Lsaarland/cispa/artist/codelib/CodeLib;log(Ljava/lang/String;)V");

// FIELDS ///////////////////////////////////
const string GunshopCodeLib::_F_CODECLASS_INSTANCE =
    "Lsaarland/cispa/artist/codelib/CodeLib;INSTANCE";

// CLASSES //////////////////////////////////
const string GunshopCodeLib::_C_JAVA_LANG_OBJECT =
    "Ljava/lang/Object;";
const string GunshopCodeLib::_C_CODECLASS =
    "Lsaarland/cispa/artist/codelib/CodeLib;";
const string GunshopCodeLib::_C_JAVA_LANG_STRING =
    "Ljava/lang/String;";

unordered_set<string> &GunshopCodeLib::getMethods() const {
    static unordered_set<string> methods({
    GunshopCodeLib::OPENCONNECTION,
    GunshopCodeLib::LOG,
    });
    return methods;
}

string &GunshopCodeLib::getInstanceField() const {
    static string instanceField = GunshopCodeLib::_F_CODECLASS_INSTANCE;
    return instanceField;
}

string &GunshopCodeLib::getCodeClass() const {
    static string codeClass = GunshopCodeLib::_C_CODECLASS;
    return codeClass;
}


// CODE_GENERATOR END //////////////////////////////////////////////////////////////////////////////////////////////////


