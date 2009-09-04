package com.fahister.ecb;

public class RoleInfo {
	private static final String NULL_KEY = "NullRole";

	private String roleId;
	private String roleName;
	private String remark;
	private Boolean active;

	public String getRoleId() {
		return roleId;
	}

	public void setRoleId(String roleId) {
		this.roleId = roleId;
	}

	public String getRoleName() {
		return roleName;
	}

	public void setRoleName(String roleName) {
		this.roleName = roleName;
	}

	public String getRemark() {
		return remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

	public Boolean getActive() {
		return active;
	}

	public void setActive(Boolean active) {
		this.active = active;
	}

	public RoleInfo() {
	}

	public RoleInfo(String roleId, String roleName) {
		this.roleId = roleId;
		this.roleName = roleName;
		this.active = true;
	}

	public static RoleInfo getNullObject() {
		return new RoleInfo(NULL_KEY, NULL_KEY);
	}

	public Boolean IsNull() {
		return (this.roleId == NULL_KEY);
	}
}