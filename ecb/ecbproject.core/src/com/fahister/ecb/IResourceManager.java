package com.fahister.ecb;

public interface IResourceManager {
	String GetString(String key);

	String GetString(String key, int languageType);
}