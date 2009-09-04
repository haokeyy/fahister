package com.fahister.ecb;

import java.util.*;

public interface IRoles {
	Boolean IsRoleExists(String roleId);

	RoleInfo GetRole(String roleId);

	List<RoleInfo> GetAllRoles();

	Boolean IsUserInRole(String userId, String roleId);

	List<UserInfo> GetUsersInRole(String roleId);

	List<RoleInfo> GetRolesForUser(String userId);

	void AddUserToRole(String userId, String roleId);

	void RemoveUserFromRole(String userId, String roleId);

	void AddRole(RoleInfo roleInfo);

	void UpdateRole(RoleInfo roleInfo);

	void DeleteRole(String roleId);
}
