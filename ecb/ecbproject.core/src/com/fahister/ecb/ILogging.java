package com.fahister.ecb;

public interface ILogging {
	void Debug(String message);

	void Debug(String message, Exception ex);

	void Debug(String category, String message);

	void Debug(String category, String message, Exception ex);

	void Warn(String message);

	void Warn(String message, Exception ex);

	void Warn(String category, String message);

	void Warn(String category, String message, Exception ex);

	void Info(String message);

	void Info(String message, Exception ex);

	void Info(String category, String message);

	void Info(String category, String message, Exception ex);

	void Error(String message);

	void Error(String message, Exception ex);

	void Error(String category, String message);

	void Error(String category, String message, Exception ex);

	void Fatal(String message);

	void Fatal(String message, Exception ex);

	void Fatal(String category, String message);

	void Fatal(String category, String message, Exception ex);
}
