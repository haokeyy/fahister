package com.fahister.ecb;

/// <summary>
/// ÊÚÈ¨µã
/// </summary>
public class DomainItem {
	private int domainId;
	private String domainName;
	private int parentId;
	private int displayIndex;
	private String iconPath;

	public int getDomainId() {
		return domainId;
	}

	public void setDomainId(int domainId) {
		this.domainId = domainId;
	}

	public String getDomainName() {
		return domainName;
	}

	public void setDomainName(String domainName) {
		this.domainName = domainName;
	}

	public int getParentId() {
		return parentId;
	}

	public void setParentId(int parentId) {
		this.parentId = parentId;
	}

	public int getDisplayIndex() {
		return displayIndex;
	}

	public void setDisplayIndex(int displayIndex) {
		this.displayIndex = displayIndex;
	}

	public String getIconPath() {
		return iconPath;
	}

	public void setIconPath(String iconPath) {
		this.iconPath = iconPath;
	}

	public DomainItem() {
	}

	public DomainItem(int domainId, String domainName) {
		this.domainId = domainId;
		this.domainName = domainName;
	}

}