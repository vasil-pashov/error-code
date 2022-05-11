#pragma once
#include <string>

/// Macro used for convenience in functions which return error code.
/// If the passed expression evaluates to EC::ErrorCode which indicates an
/// error it will return from the function which called this.
#define RETURN_ON_ERROR_CODE(_ErrExpr) \
	do { \
		const EC::ErrorCode _Err = _ErrExpr; \
		if(_Err.hasError()) { \
			return _Err; \
		} \
	} while(false);

/// @brief Simple struct use to wrap errors.
/// Contains error code for the error and message that describes it
/// If status is NOT zero assume that there is error
namespace EC {
	class ErrorCode {
	public:
		/// @brief Construct an error code which indicates success
		ErrorCode();
		/// @brief Construct an error code which indicates failure.
		/// This sets the status code to a non-zero integer.
		/// @param format The format string of the message
		/// @param ... The parameters to be filled in the format string
		explicit ErrorCode(const char* format, ...);
		/// @brief Construct an error code, with explicit error status and message.
		/// @param status The status code of the error.
		/// @param format The format string of the message
		/// @param ... The parameters to be filled in the format string
		ErrorCode(int status, const char* format, ...);
		/// @brief Return the status of the ErrorCode.
		/// @return If 0 it means no error, else it indicates error.
		int getStatus() const;
		/// @brief Get the formated message.
		const char* getMessage() const;
		/// @brief Check if the status indicates an error
		/// @returns true if an error has occurred (the status is not 0), false otherwise
		bool hasError() const;
	private:
		/// Take a format string and arguments and fill ErrorCode::message with
		/// the result of the formating.
		void makeFormat(const char* format, va_list argptr);
		int status;
		std::string message;
	};

	inline int ErrorCode::getStatus() const {
		return status;
	}

	inline const char* ErrorCode::getMessage() const {
		return message.c_str();
	}

	inline bool ErrorCode::hasError() const {
		return status != 0;
	}
}
