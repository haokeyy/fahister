package com.fahister.ecb;

public final class SystemInfo {
	// / <summary>
	// / Gets the base directory for this Application
	// / </summary>
	public static String getApplicationBaseDirectory() {
		return System.getenv("");
	}

	// / <summary>
	// / Gets the ID of the current thread.
	// / </summary>
	public static long getCurrentThreadId() {
		return Thread.currentThread().getId();
	}

	// / <summary>
	// / Get the host name or machine name for the current machine
	// / </summary>
	public static String getHostName() {
		return System.getenv("");
	}

}
