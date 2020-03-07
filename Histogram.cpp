#include "Histogram.h"

namespace wrap
{
	namespace ImageWhiten
	{
		Histogram::Histogram() {
			_histSize.push_back(256);
			_histSize.push_back(256);
			_histSize.push_back(256);
			_histRange.push_back(0);
			_histRange.push_back(180);
			_histRange.push_back(0);
			_histRange.push_back(256);
			_histRange.push_back(0);
			_histRange.push_back(256);
		}

		float Histogram::compareHist(Histogram h2, int method = NORM_L1)
		{

			Mat H1 = this->getHist();
			Mat H2 = h2.getHist();
			const Mat* arrays[] = { &H1, &H2, 0 };
			Mat planes[2];

			//N Dimensional array iterator
			NAryMatIterator it(arrays, planes);

			int len = it.planes[0].rows*it.planes[0].cols;
			float result = 0;
			float a = 0, b = 0, s1 = 0, s2 = 0;
			float s11 = 0, s12 = 0, s22 = 0;

			for (size_t i = 0; i < it.nplanes; i++, ++it)
			{
				const float* h1 = (const float*)it.planes[0].data;
				const float* h2 = (const float*)it.planes[1].data;

				if (method == NORM_L1)
				{
					for (int j = 0; j < len; j++)
					{
						double a = abs(h1[j] - h2[j]);
						if (fabs(a) > DBL_EPSILON)
							result += a;
					}
					result = result / len;
				}
				else if (method == NORM_L2)
				{
					for (int j = 0; j < len; j++)
					{
						double a = (h1[j] - h2[j])*(h1[j] - h2[j]);
						if (fabs(a) > DBL_EPSILON)
							result += a;
					}
					result = result / len;
				}
				else if (method == CHI_SQUARED)
				{
					for (int j = 0; j < len; j++)
					{
						double a = h1[j] - h2[j];
						double b = h1[j];
						if (fabs(b) > DBL_EPSILON)
							result += a*a / b;
					}
				}
				else if (method == INTERSECTION)
				{
					for (int j = 0; j < len; j++)
						result += std::min(h1[j], h2[j]);
				}
				else if (method == BHATTACHRYA || method == BHATTACHRYA1)
				{
					for (int j = 0; j < len; j++)
					{
						double a = h1[j];
						double b = h2[j];
						result += std::sqrt(a*b);
						s1 += a;
						s2 += b;
					}
					cerr << (float)s1 << ":" << (float)s2 << endl;
				}
				else if (method == CORRELATION)
				{
					for (int j = 0; j < len; j++)
					{
						double a = h1[j];
						double b = h2[j];

						s12 += a*b;
						s1 += a;
						s11 += a*a;
						s2 += b;
						s22 += b*b;
					}
				}
				else
				{
					CV_Error(CV_StsBadArg, "Unknown comparison method");
				}
			}
			if (method == CORRELATION)
			{
				s1 = s1 / len;
				s2 = s2 / len;

				size_t total = len;
				double scale = 1. / total;
				double num = s12 - (total*s1*s2);
				double denom2 = (s11 - s1*s1*total)*(s22 - s2*s2*total);
				result = std::abs(denom2) > DBL_EPSILON ? num / std::sqrt(denom2) : 1.;
			}
			else if (method == BHATTACHRYA || method == BHATTACHRYA1)
			{
				s1 *= s2;

				s1 = fabs(s1) > FLT_EPSILON ? 1. / std::sqrt(s1) : 1.;
				if (method == BHATTACHRYA1)
					result = result*s1;
				else
					result = std::sqrt(std::max(1. - result*s1, 0.));
			}
			return result;
		}

		Mat Histogram::getHist()
		{
			return _histMat;
		}

		Mat Histogram::likeyhoodImage(Mat image)
		{
			Mat out;
			int *c = (int *)calloc(sizeof(int), _channels.size());
			for (int i = 0; i<_channels.size(); i++)
			{
				c[i] = _channels[i];
			}

			int *h = (int *)calloc(sizeof(int), _channels.size());
			for (int i = 0; i<_channels.size(); i++)
			{
				h[i] = _histSize[_channels[i]];
			}

			float **ranges = (float **)calloc(sizeof(float*), _channels.size());
			int size = _channels.size();
			for (int i = 0; i<size; i++)
			{
				float *x = (float *)calloc(sizeof(float), 2);
				int index = 2 * _channels[i];
				x[0] = _histRange[index];
				x[1] = _histRange[index + 1];
				ranges[i] = x;
			}
			image.copyTo(out);
			out.convertTo(out, CV_32FC(1), 1, 0);
			out.setTo(cv::Scalar::all(0));
			Mat i2;
			cv::normalize(_histMat, i2, 0, 255, NORM_MINMAX);
			cv::calcBackProject(&image, 1, c, i2, out, (const float **)ranges, 1, true);
			out.convertTo(out, CV_8UC(1), 1.0, 0);
			return out;
		}

		void Histogram::setMask(Mat mask)
		{
			mask.copyTo(_mask);
		}

		void Histogram::mergeHistogram(Mat hist, float factor)
		{
			if (_histMat.empty())
			{
				hist.copyTo(_histMat);
			}
			else
			{
				_histMat = factor*_histMat + (1 - factor)*hist;
			}
		}

		Mat Histogram::BuildHistogram(Mat srcImage, bool accumulate) {
			cv::Mat histMat;
			//compute  histogram
			int *c = (int *)calloc(sizeof(int), _channels.size());
			for (int i = 0; i<_channels.size(); i++)
			{
				c[i] = _channels[i];
			}
			int *h = (int *)calloc(sizeof(int), _channels.size());
			for (int i = 0; i<_channels.size(); i++)
			{
				h[i] = _histSize[_channels[i]];
			}

			float **ranges = (float **)calloc(sizeof(float*), _channels.size());
			int size = _channels.size();
			for (int i = 0; i<size; i++)
			{
				float *x = (float *)calloc(sizeof(float), 2);
				int index = 2 * _channels[i];
				x[0] = _histRange[index];
				x[1] = _histRange[index + 1];
				ranges[i] = x;
			}
			if (accumulate == true)
				_histMat.copyTo(histMat);

			cv::calcHist(&srcImage, 1, c, _mask, histMat, _channels.size(), h, (const float **)ranges, true, accumulate);
			for (int i = 0; i<size; i++)
			{
				free(ranges[i]);
			}
			free(ranges);
			free(c);
			free(h);
			if (accumulate == true)
				histMat.copyTo(_histMat);
			return histMat;
		}

		Mat Histogram::drawHist()
		{
			if (_histMat.channels()>1)
				cvError(1, __FUNCTION__, "Only for 1D histograms", __FILE__, __LINE__);

			int w = 400; int h = 400;
			Mat histImage(h, w, CV_8UC3, Scalar(0, 0, 0));
			Mat i2;
			cv::normalize(_histMat, i2, 0, 255, CV_MINMAX);

			for (int i = 0; i < _histSize[0]; i++)
			{
				int bin_w = cvRound((double)w / _histSize[0]);
				rectangle(histImage, Point(i*bin_w, h), Point((i + 1)*bin_w, h - cvRound(i2.at<float>(i)*h / 255.0)), Scalar(0, 0, 255), -1);
			}
			return histImage;
		}

		Mat Histogram::drawHist(MatND _histMat)
		{
			CV_Assert(_histMat.channels() > 1);
				
			int w = 400; int h = 400;
			Mat histImage(h, w, CV_8UC3, Scalar(0, 0, 0));
			Mat i2;
			cv::normalize(_histMat, i2, 0, 255, CV_MINMAX);

			for (int i = 0; i < _histSize[0]; i++)
			{
				int bin_w = cvRound((double)w / _histSize[0]);
				rectangle(histImage, Point(i*bin_w, h), Point((i + 1)*bin_w, h - cvRound(i2.at<float>(i)*h / 255.0)), Scalar(0, 0, 255), -1);
			}
			return histImage;
		}

		void Histogram::setChannel(vector<int>channels)
		{
			_channels = channels;
		}

		void Histogram::setHistSize(vector<int> size)
		{
			_histSize = size;
		}

		void Histogram::setRange(vector<float> range)
		{
			_histRange = range;
		}

		Mat Histogram::applyKernel(int size, int type)
		{
			if (_histMat.channels()>1)
				cvError(1, __FUNCTION__, "Only for 1D histograms", __FILE__, __LINE__);
			Mat output;
			Mat input = cv::Mat(1, 3, CV_32FC1);
			Mat kernel = cv::Mat(size, 1, CV_32FC1);
			if (type == 1)
			{
				kernel.setTo(cv::Scalar::all(1));
				input.setTo(cv::Scalar::all(1));
			}
			else if (type == 2)
			{
				kernel = getGaussianKernel(256, size, CV_32FC1);
			}

			Scalar sum = cv::sum(kernel);

			cv::filter2D(_histMat, output, _histMat.depth(), kernel, Point(-1, -1), 0, BORDER_CONSTANT);
			if (type == 1)
			{
				output.convertTo(output, output.type(), 1.0 / sum[0], 0);
			}
			return output;
		}

		vector<int> Histogram::getThreshHist(Mat histMat, float s1, float s2) {

			std::vector<int> imgThresh;
			imgThresh.resize(2);

			Scalar s = cv::mean(histMat);
			float N = s[0] * histMat.cols*histMat.rows;
			float maxth = (1 - s2)*N;
			float minth = s1*N;
			int mini = 0, maxi = 0;
			float cmini = 0, cmaxi = N;
			bool lower = true, higher = true;

			for (int i = 0; i < histMat.rows; i++) {

				cmini += histMat.at<float>(i);
				cmaxi -= histMat.at<float>(histMat.rows - i - 1);

				if (cmini >= minth && lower == true) {
					mini = i;
					lower = false;
				}
				if (cmaxi <= maxth && higher == true) {
					maxi = histMat.rows - i;
					higher = false;
				}
				if (lower == false && higher == false)
					break;
			}
			imgThresh[0] = mini;
			imgThresh[1] = maxi;
			return imgThresh;

		}

		vector<int> Histogram::getThresh(Mat image, float s1, float s2) {
			Mat histMat = BuildHistogram(image);
			vector<int> imgThresh;
			imgThresh.push_back(0);
			imgThresh.push_back(255);
			float N = image.rows * image.cols;
			float maxth = (1 - s2)*N;
			float minth = s1*N;
			int mini = 0, maxi = 0;
			float cmini = 0, cmaxi = N;
			bool lower = true, higher = true;
			for (int i = 0; i < histMat.rows; i++) {
				cmini += histMat.at<float>(i);
				cmaxi -= histMat.at<float>(256 - i);
				if (cmini >= minth && lower == true) {
					mini = i;
					lower = false;
				}
				if (cmaxi <= maxth && higher == true) {
					maxi = 256 - i;
					higher = false;
				}
				if (lower == false && higher == false)
					break;
			}
			imgThresh[0] = mini;
			imgThresh[1] = maxi;
			return imgThresh;
		}
	}
}

