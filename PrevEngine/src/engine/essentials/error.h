#pragma once

#include "pch.h"

#define PV_POST_INFO(message)	GiveError(message, ErrorLevel::PV_INFO)
#define PV_POST_WARN(message)	GiveError(message, ErrorLevel::PV_WARN)
#define PV_POST_ERROR(message)	GiveError(message, ErrorLevel::PV_ERROR)
#define PV_POST_FATAL(message)	GiveError(message, ErrorLevel::PV_FATAL)

namespace prev {

	enum class ErrorLevel {
		PV_INFO,
		PV_WARN,
		PV_ERROR,
		PV_FATAL
	};

	static void GiveError(std::string message, ErrorLevel errorlevel) {

		std::string caption;
		UINT flags = 0;
		flags |= MB_OK;

		switch (errorlevel) {
		case prev::ErrorLevel::PV_INFO:
			caption = "INFO";
			flags |= MB_ICONEXCLAMATION;
			break;
		case prev::ErrorLevel::PV_WARN:
			flags |= MB_ICONWARNING;
			caption = "WARNING";
			break;
		case prev::ErrorLevel::PV_ERROR:
			flags |= MB_ICONASTERISK;
			caption = "ERROR";
			break;
		case prev::ErrorLevel::PV_FATAL:
			flags |= MB_ICONERROR;
			caption = "FATAL";
			break;
		default:
			break;
		}


		MessageBoxA(nullptr, message.c_str(), caption.c_str(), flags);
	}

}
