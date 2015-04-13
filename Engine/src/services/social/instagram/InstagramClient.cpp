#include "instagram/InstagramClient.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace instagram;
using namespace mndl::curl;

InstagramClient::InstagramClient(const string& clientID)
	:clientID(clientID),
	_loading(false),
	_needSynch(false),
	_noMore(false)
{

}

////////////////////////////////////////////////////////////////////////////
//
//					LOAD POPULAR
//
////////////////////////////////////////////////////////////////////////////


void InstagramClient::loadPopular(int count)
{
	if (canLoad())
	{
		setupLoadThread();
		mediaLoadThread = ThreadRef(new boost::thread(bind(&InstagramClient::_loadPopular, this, count)));
	}
}

void InstagramClient::_loadPopular(int count)
{
	string request = API::POPULAR + "?" +
		API::CLIENT_ID + "=" + clientID +
		"&COUNT=" + to_string(count);	

	loadMediaRequest(request);
}

////////////////////////////////////////////////////////////////////////////
//
//					LOAD BY TAG
//
////////////////////////////////////////////////////////////////////////////

void InstagramClient::loadTagMedia(const string& tagName, int count)
{
	if (canLoad())
	{
		setupLoadThread();
		mediaLoadThread = ThreadRef(new boost::thread(bind(&InstagramClient::_loadTagMedia, this, tagName, count)));
	}
}

void InstagramClient::_loadTagMedia(const string& tagName, int count)
{
	string request = API::TAGS + Utils::cp1251_to_utf8(tagName.c_str()) + "/" +
		API::MEDIA_RECENT + "?" +
		API::CLIENT_ID + "=" + clientID +
		"&COUNT=" + to_string(count);

	loadMediaRequest(request);
}

////////////////////////////////////////////////////////////////////////////
//
//					USER LOAD 
//
////////////////////////////////////////////////////////////////////////////

void InstagramClient::loadUserMedia(const string& tagName, int count)
{
	if (canLoad())
	{
		setupLoadThread();
		mediaLoadThread = ThreadRef(new boost::thread(bind(&InstagramClient::_loadUserMedia, this, tagName, count)));
	}
}

void InstagramClient::_loadUserMedia(const string& userName, int count)
{
	string request = API::USERS
		+ API::SEARCH
		+ "?q="
		+ Utils::cp1251_to_utf8(userName.c_str()) + "&" +
		API::CLIENT_ID + "=" + clientID;

	loadUsersRequest(request);
}

void InstagramClient::loadUsersRequest(const string& request)
{
	string json = Curl::get(request);
	userResponse.parse(json);

	auto data = userResponse.getData();

	if (data.empty())
	{
		_needSynch = true;
		_loading = false;
	}
	else
	{
		auto firstUser = data.front();
		console() << "firstUser::::   " << firstUser.getID() << endl;	
		loadUserPhotos(firstUser.getID());
	}
}

void InstagramClient::loadUserPhotos(const string& userID, int count)
{
	string request = API::USERS
		+ userID + "/" +
		API::MEDIA_RECENT + "?" +
		API::CLIENT_ID + "=" + clientID +
		"&COUNT=" + to_string(count);

	loadMediaRequest(request);
}

////////////////////////////////////////////////////////////////////////////
//
//					PAGINATION
//
////////////////////////////////////////////////////////////////////////////

void InstagramClient::loadNextMedia()
{
	if (canLoad())
	{
		setupLoadThread();
		mediaLoadThread = ThreadRef(new boost::thread(bind(&InstagramClient::_loadNextMedia, this)));
	}
}

void InstagramClient::_loadNextMedia()
{
	if(lastMediaResponse.getPagination().hasNextURL())
		loadMediaRequest(lastMediaResponse.getPagination().getNextURL());
	else
	{
		console() << "no more::::::::::::::  " << endl;
		_needSynch = false;
		_loading = false;
		_noMore = true;
	}
}

void InstagramClient::loadMediaRequest(const string& request)
{
	string json = Curl::get(request);
	lastMediaResponse.parse(json);
	loadImages();

	_needSynch = true;
	_loading = false;
}

void InstagramClient::setupLoadThread()
{
	synchImages.clear();
	_loading = true;
	updateCon = App::get()->getSignalUpdate().connect(std::bind(&InstagramClient::update, this));
	startLoadEvent();
}

void InstagramClient::update()
{
	if (!_loading)
	{
		if (_needSynch)
		{
			updateCon.disconnect();
			synchEvent();
		}
		else if (_noMore)
		{
			updateCon.disconnect();
			noMoreEvent();
		}	
	}
}

////////////////////////////////////////////////////////////////////////////
//
//					GETTERS
//
////////////////////////////////////////////////////////////////////////////

bool InstagramClient::isLoading() const
{
	return _loading;
}

bool InstagramClient::needSynch() const
{
	return _needSynch;
}

void InstagramClient::setSynch(bool val)
{
	_needSynch = val;
}

std::vector<ImageGraphic> InstagramClient::getImages() const
{
	return synchImages;
}

bool InstagramClient::canLoad() const
{
	return !_loading && !_needSynch;
}

void InstagramClient::loadImages()
{
	list<InstagramMedia> mediaList = lastMediaResponse.getData();

	synchImages.clear();

	for (auto image : mediaList)
	{
		ImageGraphic imageGr;
		imageGr.setLowResURL(image.getImagesList().getLowResolution().getURL());
		imageGr.setStandartResURL(image.getImagesList().getStandardResolution().getURL());
		imageGr.setSize(THUMB_SIZE);
		synchImages.push_back(imageGr);
	}
}

void InstagramClient::killLoad()
{
	if (_loading)
	{
		if (mediaLoadThread && mediaLoadThread->joinable())
			mediaLoadThread->join();
	}

	_loading = false;
	_needSynch = false;
	ci::app::console() << "KILLED!!!!!!!!!!!!!!!" << endl;
}