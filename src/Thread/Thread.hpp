/*!
 * \file
 * \brief Définition de la classe Thread.
 */

#ifndef UTILS_THREAD_HPP
#define UTILS_THREAD_HPP

#include <pthread.h>
#include <string>

#include "../Log/LoggerFactory.hpp"
#include "Mutex.hpp"

namespace utils
{
/*!
 * \brief Enumération des états des threads.
 */
enum ThreadState
{
	CREATED,
	STARTING,
	STARTED,
	STOPPED
};

/*!
 * \brief Cette classe encapsule l'implémentation des threads.
 *
 * \todo Ajouter les méthodes join() et kill().
 */
class Thread: public utils::Mutex
{
protected:

	/*!
	 * \brief Cette méthode sert de point d'entrée pour l'appel à
	 * pcreate_thread.
	 *
	 * \param object
	 *        Instance de Thread qui doit être lancé.
	 */
	static void *entryPoint(void *object);

private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref Thread.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("Thread");
		return instance;
	}

	typedef pthread_t ThreadId;

	/*!
	 * \brief Identifiant du thread lié.
	 */
	ThreadId threadId_;

	/*!
	 * \brief l'état du thread.
	 */
	ThreadState state_;

protected:

	/*!
	 * \brief Constructeur de la classe.
	 */
	Thread();

	/*!
	 * \brief Cette méthode libère le controle du thread et permet au processus
	 * appelant de traiter un autre thread.
	 */
	void yield();

	/*!
	 * \brief L'implementation de cette méthode détaille le traitement
	 * réalisé par ce thread.
	 */
	virtual void execute() = 0;

public:

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~Thread()
	{
	}

	/*!
	 * \brief Lancement du thread.
	 *
	 * Cette méthode initialise le thread et appelle la méthode ::execute().
	 */
	bool start(std::string name);

	/*!
	 * \brief Cette méthode retourne l'état du thread.
	 *
	 * \return l'état du thread.
	 */
	inline ThreadState state()
	{
		lock();
		ThreadState s = state_;
		unlock();
		return s;
	}

	inline void setState(ThreadState state)
	{
		lock();
		state_ = state;
		unlock();
	}
	/*!
	 * \brief Indique si le thread est terminé ou pas démarré.
	 *
	 * \return \c true si le thread est terminé ou simplement créé.
	 */
	inline bool isFinished()
	{
		// printf("Thread.hpp:  is finished()\n");
		return (state() == utils::STOPPED || state() == utils::CREATED);
	}

	/*
	 * Will not return until the internal thread has exited.
	 */
	void waitForEnd()
	{
		pthread_join(threadId_, NULL);
	}
};
}

#endif

//eXAMPLE
/*class MyThreadClass
 {
 public:
 MyThreadClass() {}
 virtual ~MyThreadClass() {}

 //Returns true if the thread was successfully started, false if there was an error starting the thread
 bool StartInternalThread()
 {
 return (pthread_create(&_thread, NULL, InternalThreadEntryFunc, this) == 0);
 }

 // Will not return until the internal thread has exited.
 void WaitForInternalThreadToExit()
 {
 (void) pthread_join(_thread, NULL);
 }

 protected:
 // Implement this method in your subclass with the code you want your thread to run.
 virtual void InternalThreadEntry() = 0;

 private:
 static void * InternalThreadEntryFunc(void * This) {((MyThreadClass *)This)->InternalThreadEntry(); return NULL;}

 pthread_t _thread;
 };
 */
