package com.user;

public class Users implements java.io.Serializable {

	private Integer userId;
	private String username;
	private String password;
	private String email;
	private int type;

	public Users() {
	}

	public Users(String username, String password, String email, int type) {
		this.username = username;
		this.password = password;
		this.email = email;
		this.type = type;
	}

	public Integer getUserId() {
		return this.userId;
	}

	public void setUserId(Integer userId) {
		this.userId = userId;
	}

	public String getUsername() {
		return this.username;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	public String getPassword() {
		return this.password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public String getEmail() {
		return this.email;
	}

	public void setEmail(String email) {
		this.email = email;
	}

	public int getType() {
		return this.type;
	}

	public void setType(int type) {
		this.type = type;
	}
}
