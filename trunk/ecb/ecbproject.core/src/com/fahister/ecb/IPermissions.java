package com.fahister.ecb;

import java.util.*;

/// <summary>
/// 权限管理
/// </summary>
public interface IPermissions {
	List<MenuInfo> GetUserMenus(String userId);

	PageInfo GetPageByPath(String pageUrl);

	List<FunctionItem> GetPageAllFunctionItems(int pageId);

	List<FunctionItem> GetUserPagePermissions(String userId, int pageId);

	// 权限维护方法
	List<DomainItem> GetAllDomains();

	void AddRolePermission(String roleId, int domainId);

	void RemoveRolePermission(String roleId, int domainId);

	List<DomainItem> GetRoleAllPermissions(String roleId);
}