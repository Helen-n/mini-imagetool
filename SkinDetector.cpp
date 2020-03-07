#include "SkinDetector.h"

namespace wrap
{
	namespace ImageWhiten
	{
		SkinDetector::SkinDetector()
		{
			_hueLower = 3;
			_hueUpper = 33;
			lower = Scalar(3, 50, 50);
			higher = Scalar(33, 255, 255);
			_mergeFactor = 0.95;
			channels.resize(1);
			channels[0] = 0;
			h.setChannel(channels);

			histSize.resize(1);
			histSize[0] = 30;
			h.setHistSize(histSize);

			ranges.resize(2 * channels.size());
			ranges[0] = 0;
			ranges[1] = 30;
			h.setRange(ranges);
		}

		void SkinDetector::skinMask(Mat image, Mat &mask)
		{
			cv::cvtColor(image, image, CV_BGR2HSV);
			cv::inRange(image, lower, higher, mask);
			vector<Mat> ch;
			Mat hue;
			cv::split(image, ch);
			ch[0].copyTo(hue);
			h.setMask(mask);
			Mat hist = h.BuildHistogram(hue, false);
			cv::normalize(hist, hist, 0, 1, NORM_MINMAX);
			h.setHist(hist);
			vector<int> range1 = h.getThreshHist(hist, 0.05, 0.05);
			_hueLower = range1[0];
			_hueUpper = range1[1];
			Mat mmask = Mat();
			if (!p1.empty())
			{
				Mat motion;
				absdiff(p1, ch[2], motion);
				inRange(motion, Scalar(8, 0, 0), Scalar(255, 0, 0), mmask);
				erode(mmask, mmask, Mat());
				dilate(mmask, mmask, Mat());
			}
			if (!mmask.empty())
				cv::bitwise_and(mask, mmask, mmask);
			h.setMask(mmask);
			Mat shist = h.BuildHistogram(hue, false);
			normalize(shist, shist, 0, 1, NORM_MINMAX);
			h.mergeHistogram(shist, 0.02);
			hist = h.getHist();
			h.getThreshHist(hist, 0.05, 0.05);
			_hueLower = range1[0];
			_hueUpper = range1[1];
			MatIterator_<uchar> it = mask.begin<uchar>(), it_end = mask.end<uchar>();
			MatIterator_<uchar> it1 = hue.begin<uchar>();
			for (; it != it_end; ++it, ++it1++)
			{
				if (!*it)			
					continue;

				if (!(*it1 >= _hueLower && *it1 <= _hueUpper))
					(*it) = 0;
			}
			ch[2].copyTo(p1);
			cvtColor(image, image, CV_HSV2BGR);
		}
	}
}


