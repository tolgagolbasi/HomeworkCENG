package com.video;

import java.util.Date;

public class VideoAdministration {

	private Integer videoId;
	private String videoName;
	private int totalVotes;
	private float average;
	private Date uploadTime;
	private String username;
	private int status;

	public VideoAdministration(Integer videoId, String videoName, int totalVotes, float average, Date uploadTime, String username, int status) {
		this.videoId = videoId;
		this.videoName = videoName;
		this.totalVotes = totalVotes;
		this.average = average;
		this.uploadTime = uploadTime;
		this.username = username;
		this.status = status;
	}

	public float getAverage() {
		return average;
	}

	public void setAverage(float average) {
		this.average = average;
	}

	public int getStatus() {
		return status;
	}

	public void setStatus(int status) {
		this.status = status;
	}

	public int getTotalVotes() {
		return totalVotes;
	}

	public void setTotalVotes(int totalVotes) {
		this.totalVotes = totalVotes;
	}

	public Date getUploadTime() {
		return uploadTime;
	}

	public void setUploadTime(Date uploadTime) {
		this.uploadTime = uploadTime;
	}

	public String getUsername() {
		return username;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	public Integer getVideoId() {
		return videoId;
	}

	public void setVideoId(Integer videoId) {
		this.videoId = videoId;
	}

	public String getVideoName() {
		return videoName;
	}

	public void setVideoName(String videoName) {
		this.videoName = videoName;
	}
}
