#pragma once
#include "Type.h"
#include "MethodType.h"

#define TYPE_NAME L"Class"

namespace inspectit {
	namespace types {

		class ClassType : public Type
		{
		private:
			typedef Type super;

			std::vector<std::wstring> superClasses;
			std::vector<std::wstring> realizedInterfaces;

			std::vector<std::shared_ptr<MethodType>> methods;

		public:
			ClassType();
			~ClassType();

			std::vector<std::wstring> getSuperClasses();
			void addSuperClass(std::wstring superClass);
			std::vector<std::wstring> getRealizedInterfaces();
			void addInterface(std::wstring interfaceName);

			std::vector<std::shared_ptr<MethodType>> getMethods();
			void addMethod(std::shared_ptr<MethodType> method);

			std::wstring typeName();

			web::json::value toJson();
		};

	}
}