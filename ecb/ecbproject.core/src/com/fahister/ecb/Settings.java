package com.fahister.ecb;

public class Settings {
	public static String GetItem(String key) {
		return System.getProperty(key);
	}

	public static String getSqlMapperPath() {
		return GetItem(ConfigurationKeys.SqlMapperPath);
	}

	public class ConfigurationKeys {
		public static final String SqlMapperPath = "sqlMapPath";
	}
}
