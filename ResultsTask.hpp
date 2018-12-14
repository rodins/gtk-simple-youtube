// ResultsTask.hpp

class ResultsTask {
	ResultsView *view;
	ResultsNet *net;
	
	GThreadPool *threadPool;
	bool isThreadStarted;
	public:
	ResultsTask(ResultsView *view, ResultsNet *net) {
		this->view = view;
		this->net = net;
		
		threadPool = g_thread_pool_new(ResultsTask::task,
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
			view->showError(net->isPaging());
		}
	}
	
	static void task(gpointer arg, gpointer arg1) {
		// On pre execute
		gdk_threads_enter();
		ResultsTask *task = (ResultsTask*)arg1;
		task->view->showLoadingIndicator(task->net->isPaging());
	    gdk_threads_leave();
	    // async part
		CURLcode res = task->net->getResultsFromNet();
		gdk_threads_enter();
		task->onPostExecute(res);                                 
		gdk_threads_leave();
	}   
};
