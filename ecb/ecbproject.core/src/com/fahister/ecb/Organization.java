package com.fahister.ecb;

import java.util.*;

public class Organization {

	private static final String NULL_KEY = "NullOrganization";

	private String orgId;
	private String parentOrgId;
	private String orgCode;
	private String orgName;
	private Date starttime;
	private Date endtime;
	private String remark;
	private int status;

	public String getOrgId() {
		return orgId;
	}

	public void setOrgId(String orgId) {
		this.orgId = orgId;
	}

	public String getParentOrgId() {
		return parentOrgId;
	}

	public void setParentOrgId(String parentOrgId) {
		this.parentOrgId = parentOrgId;
	}

	public String getOrgCode() {
		return orgCode;
	}

	public void setOrgCode(String orgCode) {
		this.orgCode = orgCode;
	}

	public String getOrgName() {
		return orgName;
	}

	public void setOrgName(String orgName) {
		this.orgName = orgName;
	}

	public Date getStarttime() {
		return starttime;
	}

	public void setStarttime(Date starttime) {
		this.starttime = starttime;
	}

	public Date getEndtime() {
		return endtime;
	}

	public void setEndtime(Date endtime) {
		this.endtime = endtime;
	}

	public String getRemark() {
		return remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

	public int getStatus() {
		return status;
	}

	public void setStatus(int status) {
		this.status = status;
	}

	public Organization(String orgId, String orgName) {
		this.orgId = orgId;
		this.orgName = orgName;
	}

	public static Organization getNullObject() {
		return new Organization(NULL_KEY, NULL_KEY);
	}

	public Boolean IsNull() {
		return (this.getOrgId() == NULL_KEY);
	}
}