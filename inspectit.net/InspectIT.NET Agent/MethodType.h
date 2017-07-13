#pragma once
#include "basehdr.h"
#include "JsonSendable.h"

namespace inspectit {
	namespace types {

		class MethodType
		{
		private:
			std::wstring name;
			JAVA_INT modifiers;
			std::wstring returnType;
			std::vector<std::wstring> parameters;
			std::vector<std::wstring> exceptions;
			std::vector<std::wstring> annotations;

			/* Cannot use FunctionID, since we need this before the function was loaded */
			mdMethodDef methodDef = 0;

		public:
			MethodType();
			~MethodType();

			std::wstring getName();
			void setName(std::wstring name);
			JAVA_INT getModifiers();
			void setModifiers(JAVA_INT modifiers);
			std::wstring getReturnType();
			void setReturnType(std::wstring returnType);
			std::vector<std::wstring> getParameters();
			void addParameter(std::wstring parameter);
			std::vector<std::wstring> getExceptions();
			void addException(std::wstring exception);
			std::vector<std::wstring> getAnnotations();
			void addAnnotation(std::wstring annotation);

			mdMethodDef getMethodDef();
			void setMethodDef(mdMethodDef methodDef);

			web::json::value toJson();
		};

	}
}