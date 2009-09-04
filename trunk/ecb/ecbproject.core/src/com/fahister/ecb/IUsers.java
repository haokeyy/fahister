package com.fahister.ecb;

import java.util.*;

public interface IUsers {
	Boolean IsUserExists(String userId);

	UserInfo GetUser(String userId);

	UserInfo GetUserByAlias(String userAlias);

	List<UserInfo> GetAllUser();

	ValidateResult ValidateUser(String loginAlias, String password);

	void ChangePassword(String userId, String newPassword);

	void AddUser(UserInfo userInfo);

	void DeleteUser(String userId);

	void UpdateUser(UserInfo userInfo);

	public enum ValidateResult {
		OK, NotExists, InvalidPassword, UserStoped
	}
}
