package com.video;

import com.xuggle.mediatool.IMediaReader;
import com.xuggle.mediatool.MediaListenerAdapter;
import com.xuggle.mediatool.ToolFactory;
import com.xuggle.mediatool.event.IVideoPictureEvent;
import com.xuggle.xuggler.Global;
import com.xuggle.xuggler.IContainer;
import java.awt.AlphaComposite;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;
import java.io.File;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import javax.imageio.ImageIO;

public class ThumbnailGenerator extends MediaListenerAdapter {

	public static final double SECONDS_BETWEEN_FRAMES = 5;
	public static final long MICRO_SECONDS_BETWEEN_FRAMES = (long) (Global.DEFAULT_PTS_PER_SECOND * SECONDS_BETWEEN_FRAMES);
	private static long mLastPtsWrite = Global.NO_PTS;
	private int mVideoStreamIndex = -1;
	public String output;
	public String filename;
	public boolean exit = false;
	private static final int IMG_WIDTH = 480;
	private static final int IMG_HEIGHT = 320;
	private String absolutepath;

	public static void main(String[] args) {
	}

	public ThumbnailGenerator(String filename, String output) {

		this.filename = filename;
		this.output = output;

	}

	public String fGenerateThumbnail() {

		IMediaReader reader = ToolFactory.makeReader(filename);
		reader.setBufferedImageTypeToGenerate(BufferedImage.TYPE_3BYTE_BGR);
		reader.addListener(this);
		while ((reader.readPacket() == null) && !exit) {
			do {
			} while (false);
		}
		return absolutepath;
	}

	public void onVideoPicture(IVideoPictureEvent event) {
		try {
			if (event.getStreamIndex() != mVideoStreamIndex) {
				if (-1 == mVideoStreamIndex) {
					mVideoStreamIndex = event.getStreamIndex();
				} else {
					return;
				}
			}
			if (mLastPtsWrite == Global.NO_PTS) {
				mLastPtsWrite = event.getTimeStamp() - MICRO_SECONDS_BETWEEN_FRAMES;
			}
			if (event.getTimeStamp() - mLastPtsWrite >= MICRO_SECONDS_BETWEEN_FRAMES) {
				if (event.getTimeStamp() >= 5000000) {
					File directory = new File(output);
					File file = File.createTempFile("frame", ".png", directory);

					BufferedImage originalImage = event.getImage();
					int type = originalImage.getType() == 0 ? BufferedImage.TYPE_INT_ARGB : originalImage.getType();

					BufferedImage resizedImage = resizeImageWithHint(originalImage, type);

					ImageIO.write(resizedImage, "png", file);
					double seconds = ((double) event.getTimeStamp())
							/ Global.DEFAULT_PTS_PER_SECOND;
//                    System.out.printf("at elapsed time of %6.3f seconds wrote: %s\n",
//                            seconds, file);
					exit = true;
					absolutepath = file.getAbsolutePath();
				}
				mLastPtsWrite += MICRO_SECONDS_BETWEEN_FRAMES;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private static BufferedImage resizeImageWithHint(BufferedImage originalImage, int type) {

		BufferedImage resizedImage = new BufferedImage(IMG_WIDTH, IMG_HEIGHT, type);
		Graphics2D g = resizedImage.createGraphics();
		g.drawImage(originalImage, 0, 0, IMG_WIDTH, IMG_HEIGHT, null);
		g.dispose();
		g.setComposite(AlphaComposite.Src);

		g.setRenderingHint(RenderingHints.KEY_INTERPOLATION,
				RenderingHints.VALUE_INTERPOLATION_BILINEAR);
		g.setRenderingHint(RenderingHints.KEY_RENDERING,
				RenderingHints.VALUE_RENDER_QUALITY);
		g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
				RenderingHints.VALUE_ANTIALIAS_ON);

		return resizedImage;
	}
}