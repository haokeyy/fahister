package com.fahister.ecb;

/// <summary>
/// “≥√Ê–≈œ¢
/// </summary>
public class PageInfo {
	private static final int NULLKEY = -1;
	private int pageId;
	private String pageName;
	private String pageUrl;
	private String location;
	private String remark;
	private Boolean controlled;
	private Boolean active;

	public int getPageId() {
		return pageId;
	}

	public void setPageId(int pageId) {
		this.pageId = pageId;
	}

	public String getPageName() {
		return pageName;
	}

	public void setPageName(String pageName) {
		this.pageName = pageName;
	}

	public String getPageUrl() {
		return pageUrl;
	}

	public void setPageUrl(String pageUrl) {
		this.pageUrl = pageUrl;
	}

	public String getLocation() {
		return location;
	}

	public void setLocation(String location) {
		this.location = location;
	}

	public String getRemark() {
		return remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

	public Boolean getControlled() {
		return controlled;
	}

	public void setControlled(Boolean controlled) {
		this.controlled = controlled;
	}

	public Boolean getActive() {
		return active;
	}

	public void setActive(Boolean active) {
		this.active = active;
	}

	public PageInfo() {
	}

	public PageInfo(int pageId, String pageName, String pageUrl) {
		this.pageId = pageId;
		this.pageName = pageName;
		this.pageUrl = pageUrl;
	}

	public static PageInfo getNullObject() {
		return new PageInfo(NULLKEY, "", "");
	}

	public Boolean IsNull() {
		return this.pageId == NULLKEY;
	}
}
