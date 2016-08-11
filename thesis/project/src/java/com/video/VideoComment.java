package com.video;

import java.util.Date;

public class VideoComment {

	private String username;
	private String comment;
	private Date commentTime;

	public String getComment() {
		return comment;
	}

	public void setComment(String comment) {
		this.comment = comment;
	}

	public String getUsername() {
		return username;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	public Date getCommentTime() {
		return commentTime;
	}

	public void setCommentTime(Date commentTime) {
		this.commentTime = commentTime;
	}

	public VideoComment(String username, String comment, Date commentTime) {
		this.username = username;
		this.comment = comment;
		this.commentTime = commentTime;
	}
}
