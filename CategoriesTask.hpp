// CategoriesTask.hpp

//TODO: make one task instead of categories task and results task
class CategoriesTask {
	CategoriesView *view;
	CategoriesNet *net;
	
	GThreadPool *threadPool;
	bool isThreadStarted;
	
	public:
	CategoriesTask(CategoriesView *view, CategoriesNet *net) {
		this->view = view;
		this->net = net;
		
		threadPool = g_thread_pool_new(CategoriesTask::task,
	                                   this,
	                                   1, // Run one thread at the time
	                                   false,
	                                   NULL);
	    isThreadStarted = false;
	}
	
	void start() {
		if(!isThreadStarted) {
			isThreadStarted = true;
		    g_thread_pool_push(threadPool, (gpointer)1, NULL);
		}
	}
	
	private:
	
	void onPostExecute(CURLcode res) {
		isThreadStarted = false;
		if(res == CURLE_OK) {
			view->showData();
		}else {
			cout << curl_easy_strerror(res) << endl;
			view->showError();
		}
	}
	
	static void task(gpointer arg, gpointer arg1) {
		// On pre execute
		gdk_threads_enter();
		CategoriesTask *task = (CategoriesTask*)arg1;
		task->view->showLoadingIndicator();
	    gdk_threads_leave();
	    // async part
		CURLcode res = task->net->getDataFromNet();
		gdk_threads_enter();
		task->onPostExecute(res);                                 
		gdk_threads_leave();
	}
	
};
