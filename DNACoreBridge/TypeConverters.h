#define _WINSOCKAPI_
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#pragma once

#include <string>
#include <vector>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Collections::Generic;

namespace DNACoreBridge {

    /**
     * Static helper class for converting between native C++ and .NET types
     */
    public ref class TypeConverters abstract sealed {
    public:
        // ===== STRING CONVERSIONS =====

        /**
         * Convert . NET String^ to std::string
         */
        static std::string ToStdString(String^ managedString) {
            if (managedString == nullptr) {
                return std::string();
            }
            return msclr::interop::marshal_as<std::string>(managedString);
        }

        /**
         * Convert std::string to . NET String^
         */
        static String^ ToManagedString(const std::string& nativeString) {
            return gcnew String(nativeString.c_str());
        }

        // ===== VECTOR CONVERSIONS =====

        /**
         * Convert std::vector<std::string> to List<String^>^
         */
        static List<String^>^ ToManagedList(const std::vector<std::string>& nativeVector) {
            auto managedList = gcnew List<String^>();
            for (const auto& item : nativeVector) {
                managedList->Add(ToManagedString(item));
            }
            return managedList;
        }

        /**
         * Convert List<String^>^ to std::vector<std::string>
         */
        static std::vector<std::string> ToNativeVector(List<String^>^ managedList) {
            std::vector<std::string> nativeVector;
            if (managedList != nullptr) {
                for each (String ^ item in managedList) {
                    nativeVector.push_back(ToStdString(item));
                }
            }
            return nativeVector;
        }
    };

} // namespace DNACoreBridge