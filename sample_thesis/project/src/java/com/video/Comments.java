package com.video;

import java.util.Date;

public class Comments implements java.io.Serializable {

	private Integer commentId;
	private int videoId;
	private int userId;
	private Date commentTime;
	private String comment;

	public Comments() {
	}

	public Comments(int videoId, int userId, Date commentTime, String comment) {
		this.videoId = videoId;
		this.userId = userId;
		this.commentTime = commentTime;
		this.comment = comment;
	}

	public Integer getCommentId() {
		return this.commentId;
	}

	public void setCommentId(Integer commentId) {
		this.commentId = commentId;
	}

	public int getVideoId() {
		return this.videoId;
	}

	public void setVideoId(int videoId) {
		this.videoId = videoId;
	}

	public int getUserId() {
		return this.userId;
	}

	public void setUserId(int userId) {
		this.userId = userId;
	}

	public Date getCommentTime() {
		return this.commentTime;
	}

	public void setCommentTime(Date commentTime) {
		this.commentTime = commentTime;
	}

	public String getComment() {
		return this.comment;
	}

	public void setComment(String comment) {
		this.comment = comment;
	}
}
