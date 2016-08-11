package com.user;

import java.io.IOException;
import java.util.List;
import javax.faces.application.FacesMessage;
import javax.faces.context.FacesContext;
import javax.faces.event.ActionEvent;
import org.apache.log4j.Logger;

import org.springframework.orm.hibernate3.support.HibernateDaoSupport;
import org.springframework.web.servlet.support.RequestContext;

public class UserLogin extends HibernateDaoSupport {

	private static final Logger logger = Logger.getLogger(UserLogin.class);
	private boolean logged;
	private int userid;
	private String username;
	private String password;
	private int usertype;

	public boolean isLogged() {
		return logged;
	}

	public void setLogged(boolean logged) {
		this.logged = logged;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public int getUserid() {
		return userid;
	}

	public void setUserid(int userid) {
		this.userid = userid;
	}

	public String getUsername() {
		return username;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	public int getUsertype() {
		return usertype;
	}

	public void setUsertype(int usertype) {
		this.usertype = usertype;
	}

	/**
	 * Logout from system.
	 *
	 *
	 */
	public void logout() {
		logged = false;
		username = "";
		password = "";
		logger.debug("USER " + userid + " LOGOUT");
		userid = -1;
		usertype = 0;
		FacesContext fc = FacesContext.getCurrentInstance();
		try {
			fc.getExternalContext().redirect("index.xhtml");
		} catch (IOException ex) {
		}
	}

	/**
	 * Checks username and password.
	 *
	 * @param uname Username
	 * @param pword Password
	 *
	 * @return true if user exists
	 */
	public boolean checklogin(String uname, String pword) {
		String[] a = {"username", "password"};
		String b[] = new String[2];
		b[0] = uname;
		b[1] = pword;
		List<Users> result = getHibernateTemplate().findByNamedParam("from Users user where user.username= :username and user.password= :password and user.type<>99", a, b);
		if (result.size() > 0) {
			userid = result.get(0).getUserId();
			usertype = result.get(0).getType();
			logged = true;
			logger.debug("USER " + userid + " LOGIN");
			password = "";
			return true;
		} else {
			logger.debug("WRONG USERNAME [" + uname + "] OR PASSWORD [" + pword + "]");
			return false;
		}
	}

	/**
	 * Login to system.
	 *
	 *
	 * @return ok if username and password true.
	 */
	public String fLogin() {
		if (checklogin(username, password)) {
			return "ok";
		} else {
			FacesMessage message = new FacesMessage("Login Error. Wrong Username or Password");
			FacesContext context = FacesContext.getCurrentInstance();
			context.addMessage(null, message);

			return "";
		}
	}
}
