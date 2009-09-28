package com.fahister.ecb;

/// <summary>
/// ¹¦ÄÜµã
/// </summary>
public class FunctionItem {

	public static final String ViewPage = "ViewPage";

	private int functionId;
	private String functionName;
	private int pageId;
	private String tag;
	private Boolean isControl;
	private String controlId;
	private Boolean active;

	public int getFunctionId() {
		return functionId;
	}

	public void setFunctionId(int functionId) {
		this.functionId = functionId;
	}

	public String getFunctionName() {
		return functionName;
	}

	public void setFunctionName(String functionName) {
		this.functionName = functionName;
	}

	public int getPageId() {
		return pageId;
	}

	public void setPageId(int pageId) {
		this.pageId = pageId;
	}

	public String getTag() {
		return tag;
	}

	public void setTag(String tag) {
		this.tag = tag;
	}

	public Boolean getIsControl() {
		return isControl;
	}

	public void setIsControl(Boolean isControl) {
		this.isControl = isControl;
	}

	public String getControlId() {
		return controlId;
	}

	public void setControlId(String controlId) {
		this.controlId = controlId;
	}

	public Boolean getActive() {
		return active;
	}

	public void setActive(Boolean active) {
		this.active = active;
	}

	public Boolean IsViewPage() {
		return this.functionName == ViewPage;
	}
}
