package com.video;

import com.user.UserLogin;
import java.io.*;
import java.math.BigDecimal;
import java.net.MalformedURLException;
import java.net.URL;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.logging.Level;
import javax.faces.application.FacesMessage;
import javax.faces.context.ExternalContext;
import org.apache.log4j.Logger;
import javax.faces.context.FacesContext;
import javax.faces.event.ActionEvent;
import org.primefaces.event.FileUploadEvent;
import org.primefaces.event.RateEvent;
import org.primefaces.event.RowEditEvent;
import org.primefaces.model.UploadedFile;
import org.springframework.orm.hibernate3.support.HibernateDaoSupport;

public class VideoImpl extends HibernateDaoSupport {

	private static final Logger logger = Logger.getLogger(VideoImpl.class);
	int size;
	private UploadedFile file;
	String videoname;
	private int videoId;
	private int userId;
	private Date commentTime;
	private String comment;
	private String message;
	private static final int BUFFER_SIZE = 6124;
	private String folderToUpload;
	private String path;
	private String uploadedVideoName;
	private double rating;
	private int totalvotes;
	private List<VideoAdministration> adminvideolist;
	public boolean changed = false;

	public String getUploadedVideoName() {
		return uploadedVideoName;
	}

	public void setUploadedVideoName(String uploadedVideoName) {
		this.uploadedVideoName = uploadedVideoName;
	}

	public String getVideoname() {
		return videoname;
	}

	public void setVideoname(String videoname) {
		this.videoname = videoname;
	}

	public String getComment() {
		return comment;
	}

	public void setComment(String comment) {
		this.comment = comment;
	}

	public Date getCommentTime() {
		return commentTime;
	}

	public void setCommentTime(Date commentTime) {
		this.commentTime = commentTime;
	}

	public int getUserId() {
		return userId;
	}

	public void setUserId(int userId) {
		this.userId = userId;
	}

	public int getVideoId() {
		return videoId;
	}

	public void setVideoId(int videoId) {
		this.videoId = videoId;
	}

	public String getMessage() {
		return message;
	}

	public void setMessage(String message) {
		this.message = message;
	}

	public UploadedFile getFile() {
		return file;
	}

	public void setFile(UploadedFile file) {
		this.file = file;
	}

	public double getRating() {
		BigDecimal bd = new BigDecimal(rating);
		bd = bd.setScale(2, BigDecimal.ROUND_UP);
		rating = bd.doubleValue();
		return rating;
	}

	public void setRating(double rating) {
		this.rating = rating;
	}

	public int getTotalvotes() {
		return totalvotes;
	}

	public void setTotalvotes(int totalvotes) {
		this.totalvotes = totalvotes;
	}

	public List<VideoAdministration> getAdminvideolist() {
		if ((adminvideolist == null) || changed) {
			adminvideolist = fGetAdminVideos();
			changed = false;
		}
		return adminvideolist;
	}

	public void setAdminvideolist(List<VideoAdministration> adminvideolist) {

		this.adminvideolist = adminvideolist;
	}

	/**
	 * Detects parameter input.
	 *
	 * JSF method to use in any video page Also gets video name to title
	 *
	 * @return if there is parameter, HTML output of video; else redirects to index page
	 */
	public String fGetVideo() {
		FacesContext fc = FacesContext.getCurrentInstance();
		Map<String, String> params = fc.getExternalContext().getRequestParameterMap();
		size = params.size();

		if (size > 0) {
			String id = params.get("video");
			Videos result2 = getHibernateTemplate().get(Videos.class, Integer.parseInt(id));

			videoname = result2.getVideoName();
			videoId = result2.getVideoId();
			rating = result2.getAverage();
			totalvotes = result2.getTotalVotes();

			message = "";
			comment = "";
			return fFindVideo(id);
		} else {
			try {
				fc.getExternalContext().redirect("index.xhtml");
			} catch (IOException ex) {
			}
			return "";
		}

	}

	/**
	 * Gets path of video thumbnail from database
	 *
	 * @return if exists thumbnails path else default poster images path
	 */
	public String fGetThumbnail(String videoid) {
		ExternalContext extContext = FacesContext.getCurrentInstance().getExternalContext();
		String query = "from Paths path where path.videoId= " + videoid;
		List<Paths> result = getHibernateTemplate().find(query);
		String thumbnail = "";

		if ((result.size() == 2) && (result.get(1).getPath().endsWith(".png"))) {
			thumbnail = "http://" + extContext.getRequestServerName() + ":8080" + result.get(1).getPath().substring(19);

		} else {
			thumbnail = "http://dl.dropbox.com/u/21699640/poster.png";

		}
		return thumbnail;
	}

	/**
	 * Finds video from database.
	 *
	 * Gets path of video from database returns html output.
	 *
	 * @param videoid id of video that requested
	 *
	 * @return HTML output of video
	 */
	public String fFindVideo(String videoid) {
		ExternalContext extContext = FacesContext.getCurrentInstance().getExternalContext();
		String query = "from Paths path where path.videoId= " + videoid;
		List<Paths> result = getHibernateTemplate().find(query);
		String htmlOutput = "";
		if (result.size() == 2) {
			if (result.get(0).getPath().startsWith("http:")) {
				htmlOutput = "<video id=\"my_video_1\" class=\"video-js vjs-default-skin\" controls preload=\"auto\" width=\"480\" height=\"320\" poster=\"http://dl.dropbox.com/u/21699640/poster.png\" data-setup=\"{}\">"
						+ "<source src=\"" + result.get(0).getPath() + "\" type=\"video/ogg\" />"
						+ "<source src=\"" + result.get(1).getPath() + "\" type=\"video/webm\" />"
						+ "Your browser does not support the video tag."
						+ "</video>";
			} else if (result.get(0).getPath().startsWith("file:")) {
				htmlOutput = "<video id=\"my_video_1\" class=\"video-js vjs-default-skin\" controls preload=\"auto\" width=\"480\" height=\"320\" poster=\"" + "http://" + extContext.getRequestServerName() + ":8080" + result.get(1).getPath().substring(19) + "\" data-setup=\"{}\">"
						+ "<source src=\"" + "http://" + extContext.getRequestServerName() + ":8080" + result.get(0).getPath().substring(19) + "\" type=\"video/webm\" />"
						+ "Your browser does not support the video tag."
						+ "</video>";
			}
		}

		return htmlOutput;
	}

	/**
	 * Adds comment to database.
	 *
	 * Gets current time, userid, videoid and comment inserts to db.AJAX based.
	 *
	 */
	public String fAddComment() {
		Date d = new Date();
		String value = FacesContext.getCurrentInstance().
				getExternalContext().getRequestParameterMap().get("userId");
		int uid = Integer.parseInt(value);
		Comments tcomment = new Comments(videoId, uid, d, comment);
		getHibernateTemplate().saveOrUpdate(tcomment);
		message = "Comment sent";
		logger.debug("COMMENT ADD TO VIDEO " + videoId + " BY USER " + uid);
		return (null);
	}

	public List<Videos> fGetAllVideo() {
		return getHibernateTemplate().find("from Videos video where video.status=1");
	}

	/**
	 * Gets all comment from database.
	 *
	 * Gets comment from database and sets to custom videocomment class
	 *
	 * @return List of comments
	 */
	public List<VideoComment> fGetAllComments() {
		String query = "select new com.video.VideoComment(user.username,comment.comment,comment.commentTime) from Comments comment,Users user where comment.videoId= " + videoId + " and comment.userId=user.userId";
		List<VideoComment> comments = getHibernateTemplate().find(query);

		return comments;
	}

	/**
	 * Gets all video from database to admin.
	 *
	 * Gets video from database and sets to custom videoadministration class
	 *
	 * @return List of videos
	 */
	public List<VideoAdministration> fGetAdminVideos() {
		String query = "select new com.video.VideoAdministration(video.videoId,video.videoName,video.totalVotes,video.average,video.uploadTime,user.username,video.status) from Videos video,Users user where user.userId=video.userId";
		List<VideoAdministration> adminlist = getHibernateTemplate().find(query);

		return adminlist;
	}

	/**
	 * Gets video details
	 *
	 * @return Videoname
	 */
	public String fUploadedVideoDetail() {
		FacesContext fc = FacesContext.getCurrentInstance();
		Map<String, String> params = fc.getExternalContext().getRequestParameterMap();
		size = params.size();

		if (size > 0) {
			String id = params.get("video");
			String query2 = "from Videos video where video.videoId= " + id;
			List<Videos> result2 = getHibernateTemplate().find(query2);
			videoname = result2.get(0).getVideoName();
			videoId = result2.get(0).getVideoId();

			return videoname;
		} else {
			try {
				fc.getExternalContext().redirect("index.xhtml");
			} catch (IOException ex) {
			}
			return "";
		}

	}

	/**
	 * Gets uploaded video
	 *
	 * Inserts path of video to database, saves video file, generates thumbnail of videp
	 *
	 */
	public void fUploadVideo() {

		if ((file.getSize() < 52428800) && (file.getContentType().equals("video/webm"))) {
			DateFormat dateFormat = new SimpleDateFormat("yyyy.MM.dd_HH_mm_ss");
			Date d = new Date();


			String fileName = dateFormat.format(d).toString() + "_" + file.getFileName();
			byte[] bytes = file.getContents();

			ExternalContext extContext = FacesContext.getCurrentInstance().getExternalContext();

			String strFilePath = "/home/serdar/files/" + fileName;

			try {
				OutputStream fos = new BufferedOutputStream(new FileOutputStream(strFilePath), 8 * 1024);
				fos.write(bytes);
				fos.close();
			} catch (IOException ex) {
				java.util.logging.Logger.getLogger(VideoImpl.class.getName()).log(Level.SEVERE, null, ex);
			}
			FacesMessage msg = new FacesMessage("Succesful", file.getFileName() + " is uploaded as " + uploadedVideoName);
			FacesContext.getCurrentInstance().addMessage(null, msg);

			UserLogin userlogin = (UserLogin) FacesContext.getCurrentInstance().
					getExternalContext().getSessionMap().get("userLogin");
			int uid = userlogin.getUserid();

			Videos vide = new Videos(uploadedVideoName, 0, 0, d, uid, 1);
			getHibernateTemplate().saveOrUpdate(vide);
			int vid = vide.getVideoId();

			ThumbnailGenerator thumbnailgenerator = new ThumbnailGenerator(strFilePath, "/home/serdar/files");
			String thumbnail = thumbnailgenerator.fGenerateThumbnail();


			strFilePath = "file://" + strFilePath;
			thumbnail = "file://" + thumbnail;

			getHibernateTemplate().saveOrUpdate(new Paths(strFilePath, vid));
			getHibernateTemplate().saveOrUpdate(new Paths(thumbnail, vid));

			changed = true;
			FacesContext fc = FacesContext.getCurrentInstance();
			try {
				fc.getExternalContext().redirect("uploadedvideo.xhtml?video=" + vide.getVideoId());
				logger.debug("USER " + userId + " UPLOADED " + vide.getVideoId());
				uploadedVideoName = "";
			} catch (IOException ex) {
			}
		} else {
			FacesMessage msg = new FacesMessage("Error", "Only WebM supported and file must be less than 50 MB");
			FacesContext.getCurrentInstance().addMessage(null, msg);
		}
	}

	/**
	 * Checks user logged in
	 *
	 * If user logged in does nothing, else redirects to main page
	 *
	 * @param value send by UserLogin bean
	 *
	 * @return Empty string
	 */
	public String fCheckStatus(boolean value) {
		FacesContext context = FacesContext.getCurrentInstance();

		if (!value) {
			try {
				context.getExternalContext().redirect("index.xhtml");
			} catch (IOException ex) {
				java.util.logging.Logger.getLogger(VideoImpl.class.getName()).log(Level.SEVERE, null, ex);
			}
		}
		return "";
	}

	/**
	 * Gets rate for video and updates to database
	 *
	 * @param rateEvent Primefaces rating event
	 */
	public void handleRate(RateEvent rateEvent) {//sorun vote la
		Videos temp = getHibernateTemplate().get(Videos.class, videoId);
		float prev = (float) (rating * totalvotes);
		float newrating = (prev + ((Double) rateEvent.getRating()).intValue()) / (totalvotes + 1);
		temp.setAverage(newrating);
		temp.setTotalVotes(totalvotes + 1);
		rating = newrating;
		getHibernateTemplate().update(temp);
		FacesMessage message = new FacesMessage(FacesMessage.SEVERITY_INFO, "Video Rated", "You rated:" + ((Double) rateEvent.getRating()).intValue());
		logger.debug("USER " + userId + " RATED " + videoId + " " + ((Double) rateEvent.getRating()).intValue() + "/5");

		FacesContext.getCurrentInstance().addMessage(null, message);
	}

	/**
	 * Gets videos changed detail from admin interface
	 *
	 */
	public void fVideoAdminUpdate(RowEditEvent event) {
		VideoAdministration videoadmin = (VideoAdministration) event.getObject();
		Videos videotemp = getHibernateTemplate().get(Videos.class, videoadmin.getVideoId());
		videotemp.setStatus(videoadmin.getStatus());
		videotemp.setVideoName(videoadmin.getVideoName());
		getHibernateTemplate().update(videotemp);
		logger.debug("VIDEO " + videoadmin.getVideoId() + " VIDEO NAME CHANGED " + videoadmin.getVideoName() + " STATUS CHANGED " + videoadmin.getStatus());
	}
}
