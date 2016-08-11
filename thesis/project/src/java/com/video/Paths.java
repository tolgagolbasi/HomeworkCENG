package com.video;

public class Paths implements java.io.Serializable {

	private Integer pathId;
	private String path;
	private int videoId;

	public Paths() {
	}

	public Paths(String path, int videoId) {
		this.path = path;
		this.videoId = videoId;
	}

	public Integer getPathId() {
		return this.pathId;
	}

	public void setPathId(Integer pathId) {
		this.pathId = pathId;
	}

	public String getPath() {
		return this.path;
	}

	public void setPath(String path) {
		this.path = path;
	}

	public int getVideoId() {
		return this.videoId;
	}

	public void setVideoId(int videoId) {
		this.videoId = videoId;
	}
}
