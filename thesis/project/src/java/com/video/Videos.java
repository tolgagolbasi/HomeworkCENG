package com.video;

import java.util.Date;

public class Videos implements java.io.Serializable {

	private Integer videoId;
	private String videoName;
	private int totalVotes;
	private float average;
	private Date uploadTime;
	private int userId;
	private int status;

	public Videos() {
	}

	public Videos(String videoName, int totalVotes, float average, Date uploadTime, int userId, int status) {
		this.videoName = videoName;
		this.totalVotes = totalVotes;
		this.average = average;
		this.uploadTime = uploadTime;
		this.userId = userId;
		this.status = status;
	}

	public Integer getVideoId() {
		return this.videoId;
	}

	public void setVideoId(Integer videoId) {
		this.videoId = videoId;
	}

	public String getVideoName() {
		return this.videoName;
	}

	public void setVideoName(String videoName) {
		this.videoName = videoName;
	}

	public int getTotalVotes() {
		return this.totalVotes;
	}

	public void setTotalVotes(int totalVotes) {
		this.totalVotes = totalVotes;
	}

	public float getAverage() {
		return this.average;
	}

	public void setAverage(float average) {
		this.average = average;
	}

	public Date getUploadTime() {
		return this.uploadTime;
	}

	public void setUploadTime(Date uploadTime) {
		this.uploadTime = uploadTime;
	}

	public int getUserId() {
		return this.userId;
	}

	public void setUserId(int userId) {
		this.userId = userId;
	}

	public int getStatus() {
		return this.status;
	}

	public void setStatus(int status) {
		this.status = status;
	}
}
