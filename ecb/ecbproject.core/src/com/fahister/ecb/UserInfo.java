package com.fahister.ecb;

import java.util.Date;
    public class UserInfo
    {

        private static final String NULLKEY = "NullUser";

        private String userId;
        private String loginAlias;
        private String userName;
        private String email;
        private String empId;
        private String deptId;
        private Boolean active = true;
        
        public String getUserId() {
			return userId;
		}

		public void setUserId(String userId) {
			this.userId = userId;
		}

		public String getLoginAlias() {
			return loginAlias;
		}

		public void setLoginAlias(String loginAlias) {
			this.loginAlias = loginAlias;
		}

		public String getUserName() {
			return userName;
		}

		public void setUserName(String userName) {
			this.userName = userName;
		}

		public String getEmail() {
			return email;
		}

		public void setEmail(String email) {
			this.email = email;
		}

		public String getEmpId() {
			return empId;
		}

		public void setEmpId(String empId) {
			this.empId = empId;
		}

		public String getDeptId() {
			return deptId;
		}

		public void setDeptId(String deptId) {
			this.deptId = deptId;
		}

		public Boolean getActive() {
			return active;
		}

		public void setActive(Boolean active) {
			this.active = active;
		}

		public Date getLastLoginTime() {
			return lastLoginTime;
		}

		public void setLastLoginTime(Date lastLoginTime) {
			this.lastLoginTime = lastLoginTime;
		}

		private Date lastLoginTime;


        public UserInfo()
        {
        }

        public UserInfo(String userId, String loginAlias, String userName)
        {
        	this.userId = userId; 
        	this.loginAlias = loginAlias; 
        	this.userName = userName;
        	this.active = true;
        }

        public static UserInfo getNullObject()
        {
            return new UserInfo(NULLKEY, NULLKEY, NULLKEY);
        }
        
        public Boolean IsNull()
        {
            return (this.userId == NULLKEY);
        }
    }