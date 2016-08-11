package com.user;

import com.video.VideoImpl;
import java.io.IOException;
import java.util.List;
import java.util.logging.Level;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.faces.application.FacesMessage;
import javax.faces.component.UIComponent;
import javax.faces.context.FacesContext;
import javax.faces.event.ActionEvent;
import javax.faces.validator.ValidatorException;
import org.apache.log4j.Logger;
import org.primefaces.event.RowEditEvent;

import org.springframework.context.annotation.Scope;
import org.springframework.orm.hibernate3.support.HibernateDaoSupport;
import org.springframework.stereotype.Component;

public class UsersImpl extends HibernateDaoSupport {

	private static final Logger logger = Logger.getLogger(UsersImpl.class);
	private boolean loggedin;
	private int userid;
	private String username;
	private String password;
	private String password2;
	private String email;
	private int type;
	private List<Users> userlist;
	public boolean changed = false;

	public String getEmail() {
		return email;
	}

	public void setEmail(String email) {
		this.email = email;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public String getPassword2() {
		return password2;
	}

	public void setPassword2(String password2) {
		this.password2 = password2;
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

	public List<Users> getUserlist() {
		if ((userlist == null) || changed) {
			userlist = fGetUserlist();
			changed = false;
		}
		return userlist;
	}

	public void setUserlist(List<Users> userlist) {
		this.userlist = userlist;
	}

	/**
	 * Adds user to database.
	 *
	 * Default type is 1
	 *
	 * @return "ok" to redirect page
	 */
	public String fAddUser() {
		type = 1;
		Users use = new Users(username, password, email, type);
		getHibernateTemplate().saveOrUpdate(use);
		logger.debug("NEW USER REGISTERED ID " + use.getUserId());
		changed = false;
		return "ok";
	}

	/**
	 * Gets Users object
	 *
	 * @return Users
	 */
	public Users fgetUserDetails() {
		UserLogin userlogin = (UserLogin) FacesContext.getCurrentInstance().
				getExternalContext().getSessionMap().get("userLogin");
		int id = userlogin.getUserid();
		return getHibernateTemplate().get(Users.class, id);

	}

	/**
	 * Updates user information
	 *
	 * @return if successful returns ok
	 */
	public String fUpdateUser() {

		UserLogin userlogin = (UserLogin) FacesContext.getCurrentInstance().
				getExternalContext().getSessionMap().get("userLogin");
		int id = userlogin.getUserid();
		Users temp = getHibernateTemplate().get(Users.class, id);
		String a = "";
		String b = "";
		if (email.length() != 0) {
			temp.setEmail(email);
			a = a + " EMAIL";
		}
		if (password.length() != 0) {
			temp.setPassword(password);
			a = a + " PASSWORD";
		}
		if ((email.length() != 0) || (password.length() != 0)) {
			getHibernateTemplate().update(temp);
			logger.debug("USER " + id + "CHANGED " + a);
			b = b + "ok";
		}
		changed = true;
		email = "";
		return b;
	}

	/**
	 * Checks username availability
	 *
	 * @param uname Username
	 *
	 * @return status - 1 if username exists, 0 if username not exists
	 */
	public int fCheckUsername(String uname) {
		int status = 0;

		List<Users> z = getHibernateTemplate().find("from Users user where user.username= '" + uname + "'");
		if (z.size() > 0) {
			status = 1;
		} else {
			status = 0;
		}

		return status;
	}

	/**
	 * Checks email availability
	 *
	 * @param mail Email
	 *
	 * @return status - 1 if email exists, 0 if email not exists
	 */
	public int fCheckMail(String mail) {
		int status = 0;

		List<Users> z = getHibernateTemplate().find("from Users user where user.email= '" + mail + "'");
		if (z.size() > 0) {
			status = 1;
		} else {
			status = 0;
		}

		return status;
	}

	/**
	 * Custom validator for Username
	 *
	 * Checks availability and length
	 *
	 * @param context
	 * @param component
	 * @param value
	 * @throws ValidatorException
	 */
	public void fValidateUsername(FacesContext context, UIComponent component, Object value) throws ValidatorException {
		String temp = value.toString();
		int result = fCheckUsername(temp);


		if (result == 1) {
			throw new ValidatorException(new FacesMessage(
					"Username is not available"));
		}

		if ((temp.length() < 4) || (temp.length() > 25)) {
			throw new ValidatorException(new FacesMessage(
					"Username must be between 4 and 25 characters"));
		}
	}

	/**
	 * Custom validator for Email
	 *
	 * Checks availability and regular expression
	 *
	 * @param context
	 * @param component
	 * @param value
	 * @throws ValidatorException
	 */
	public void fValidateMail(FacesContext context, UIComponent component, Object value) throws ValidatorException {
		String temp = value.toString();
		int result = fCheckMail(temp);


		if (result == 1) {
			throw new ValidatorException(new FacesMessage(
					"Mail is not available"));
		}

		String re1 = "([\\w-+]+(?:\\.[\\w-+]+)*@(?:[\\w-]+\\.)+[a-zA-Z]{2,7})";	// Email Address 1

		Pattern p = Pattern.compile(re1, Pattern.CASE_INSENSITIVE | Pattern.DOTALL);
		Matcher m = p.matcher(temp);
		if (!m.find()) {
			throw new ValidatorException(new FacesMessage(
					"Invalid Email"));
		}
	}

	/**
	 * Custom validator for Email at update
	 *
	 * Checks availability and regular expression for email at update operation
	 *
	 * @param context
	 * @param component
	 * @param value
	 * @throws ValidatorException
	 */
	public void fUpdateValidateMail(FacesContext context, UIComponent component, Object value) throws ValidatorException {
		String temp = value.toString();
		int result = fCheckMail(temp);

		UserLogin userlogin = (UserLogin) FacesContext.getCurrentInstance().
				getExternalContext().getSessionMap().get("userLogin");
		int id = userlogin.getUserid();
		Users utemp = getHibernateTemplate().get(Users.class, id);

		if (!utemp.getEmail().equals(temp)) {
			if (result == 1) {
				throw new ValidatorException(new FacesMessage(
						"Mail is not available"));
			}
		}
		String re1 = "([\\w-+]+(?:\\.[\\w-+]+)*@(?:[\\w-]+\\.)+[a-zA-Z]{2,7})";	// Email Address 1

		Pattern p = Pattern.compile(re1, Pattern.CASE_INSENSITIVE | Pattern.DOTALL);
		Matcher m = p.matcher(temp);
		if (!m.find()) {
			throw new ValidatorException(new FacesMessage(
					"Invalid Email"));
		}
	}

	/**
	 * Check user is admin
	 *
	 */
	public String fCheckAdmin() {
		UserLogin userlogin = (UserLogin) FacesContext.getCurrentInstance().
				getExternalContext().getSessionMap().get("userLogin");
		int id = userlogin.getUserid();
		Users tempuser = getHibernateTemplate().get(Users.class, id);
		int usertype = tempuser.getType();
		boolean valid = false;
		if ((usertype == 3) || (usertype == 2)) {
			valid = true;
		}
		logger.debug("USERTYPE " + usertype);
		if (!valid) {
			FacesContext context = FacesContext.getCurrentInstance();
			try {
				context.getExternalContext().redirect("index.xhtml");
			} catch (IOException ex) {
				java.util.logging.Logger.getLogger(VideoImpl.class.getName()).log(Level.SEVERE, null, ex);
			}
		}

		return "";
	}

	/**
	 *
	 * @return List of users
	 */
	private List<Users> fGetUserlist() {
		return getHibernateTemplate().find("from Users");
	}

	/**
	 * Gets users changed detail from admin interface
	 *
	 */
	public void fUserAdminUpdate(RowEditEvent event) {
		Users useradmin = (Users) event.getObject();
		Users usertemp = getHibernateTemplate().get(Users.class, useradmin.getUserId());
		usertemp.setType(useradmin.getType());
		getHibernateTemplate().update(usertemp);
		logger.debug("USER " + usertemp.getUserId() + " STATUS CHANGED " + usertemp.getType());
	}
}
