package com.fahister.ecb;

/// <summary>
/// ��½�û������ĵķ�װ
/// </summary>
public class UserContext {
	private static UserContext current = null;

	public static UserContext getCurrent() {
		return current;
	}

	private UserInfo user = UserInfo.getNullObject();

	public UserInfo getUser() {
		return user;
	}

	public void setUser(UserInfo user) {
		this.user = user;
	}
}
