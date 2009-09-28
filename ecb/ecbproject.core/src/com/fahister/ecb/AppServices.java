package com.fahister.ecb;

public class AppServices {

	private static Object iocContext = null;

	public static Object GetObject(String key) {
		// iocContext = ContextRegistry.GetContext();
		return iocContext; // iocContext.GetObject(key);
	}

	private static AppServices current;

	public static AppServices getCurrent() {
		return current;
	}

	public static void setCurrent(AppServices current) {
		AppServices.current = current;
	}

	private IUsers userService;
	private IRoles roleService;
	private IPermissions permissions;
	private IResourceManager resourceManager;
	private ILogging logging;
	private GlobalConfig globalConfig;

	public IUsers getUserService() {
		return userService;
	}

	public void setUserService(IUsers userService) {
		this.userService = userService;
	}

	public IRoles getRoleService() {
		return roleService;
	}

	public void setRoleService(IRoles roleService) {
		this.roleService = roleService;
	}

	public IPermissions getPermissions() {
		return permissions;
	}

	public void setPermissions(IPermissions permissions) {
		this.permissions = permissions;
	}

	public IResourceManager getResourceManager() {
		return resourceManager;
	}

	public void setResourceManager(IResourceManager resourceManager) {
		this.resourceManager = resourceManager;
	}

	public ILogging getLogging() {
		return logging;
	}

	public void setLogging(ILogging logging) {
		this.logging = logging;
	}

	public GlobalConfig getGlobalConfig() {
		return globalConfig;
	}

	public void setGlobalConfig(GlobalConfig globalConfig) {
		this.globalConfig = globalConfig;
	}

}