#pragma once

#include "basehdr.h"
#include "JsonSendable.h"

#include <string>

#define TYPE_KEY L"type"

namespace inspectit {
	namespace types {

		class Type : inspectit::json::JsonSendable
		{
		private:
			std::wstring fqn;
			std::wstring hash;
			JAVA_INT modifiers;

		public:
			Type() {}
			virtual ~Type() {}

			std::wstring getFqn() { return fqn; }
			void setFqn(std::wstring fqn) { this->fqn = fqn; }
			std::wstring getHash() { return hash; }
			void setHash(std::wstring hash) { this->hash = hash; }
			JAVA_INT getModifiers() { return modifiers; }
			void setModifiers(JAVA_INT modifiers) { this->modifiers = modifiers; }

			virtual std::wstring typeName() = 0;

			web::json::value toJson()
			{
				web::json::value json;
				json[TYPE_KEY] = web::json::value::string(typeName());
				json[L"fqn"] = web::json::value::string(fqn);
				json[L"hash"] = web::json::value::string(hash);
				json[L"modifiers"] = web::json::value::number(modifiers);
				return json;
			}
		};

	}
}