//
//  MuiEvents.hpp
//  AudiLogo
//
//  Created by Hansi on 27.02.17.
//
//

#ifndef MuiEvents_hpp
#define MuiEvents_hpp

#include "ofMain.h"
#include <list>

namespace mui{
	class Container;
	
	template<typename EventType>
	class EventFunction{
	public:
		EventFunction(int listenerId, mui::Container * listenerElement, std::function<void()> f) :
		listenerId(listenerId), listenerElement(listenerElement), type(FTypeXXX), body(f){};
		
		EventFunction(int listenerId, mui::Container * listenerElement, std::function<void(EventType&)> f) :
		listenerId(listenerId), listenerElement(listenerElement), type(FTypeXXE), body(f){};
		
		EventFunction(int listenerId, mui::Container * listenerElement, std::function<void(mui::Container*)> f) :
		listenerId(listenerId), listenerElement(listenerElement), type(FTypeXSX), body(f){};
		
		EventFunction(int listenerId, mui::Container * listenerElement, std::function<void(mui::Container*, EventType &)> f) :
		listenerId(listenerId), listenerElement(listenerElement), type(FTypeXSE), body(f){};
		
		EventFunction(int listenerId, mui::Container * listenerElement, std::function<bool()> f) :
		listenerId(listenerId), listenerElement(listenerElement), type(FTypeCXX), body(f){};
		
		EventFunction(int listenerId, mui::Container * listenerElement, std::function<bool(EventType&)> f) :
		listenerId(listenerId), listenerElement(listenerElement), type(FTypeCXE), body(f){};
		
		EventFunction(int listenerId, mui::Container * listenerElement, std::function<bool(mui::Container*)> f) :
		listenerId(listenerId), listenerElement(listenerElement), type(FTypeCSX), body(f){};

		EventFunction(int listenerId, mui::Container * listenerElement, std::function<bool(mui::Container*, EventType&)> f) :
		listenerId(listenerId), listenerElement(listenerElement), type(FTypeCSE), body(f){};
		~EventFunction(){};
		EventFunction(EventFunction&&) = default;
		
		bool call(mui::Container * owner, EventType & evt){
			switch(type){
				case FTypeXXX: body.funcXXX(); return false;
				case FTypeXXE: body.funcXXE(evt); return false;
				case FTypeXSX: body.funcXSX(owner); return false;
				case FTypeXSE: body.funcXSE(owner,evt); return false;
				case FTypeCXX: return body.funcCXX();
				case FTypeCXE: return body.funcCXE(evt);
				case FTypeCSX: return body.funcCSX(owner);
				case FTypeCSE: return body.funcCSE(owner,evt);
				default: return false; 
			}
		}
		
		// C = is consumable (ie: returns bool)
		// S = use sender argument
		// E = use event argument
		enum FType{
			FTypeXXX = 1,
			FTypeXXE = 2,
			FTypeXSX = 3,
			FTypeXSE = 4,
			FTypeCXX = 5,
			FTypeCXE = 6,
			FTypeCSX = 7,
			FTypeCSE = 8
		};
		union FBody{
			FBody(){}
			~FBody(){}
			FBody(const std::function<void()> & funcXXX) : funcXXX(funcXXX){};
			FBody(const std::function<void(EventType&)> & funcXXE) : funcXXE(funcXXE){};
			FBody(const std::function<void(mui::Container*)> & funcXSX) : funcXSX(funcXSX){};
			FBody(const std::function<void(mui::Container*, EventType&)> & funcXSE) : funcXSE(funcXSE){};
			FBody(const std::function<bool()> & funcCXX) : funcCXX(funcCXX){};
			FBody(const std::function<bool(EventType&)> & funcCXE) : funcCXE(funcCXE){};
			FBody(const std::function<bool(mui::Container*)> & funcCSX) : funcCSX(funcCSX){};
			FBody(const std::function<bool(mui::Container*, EventType&)> & funcCSE) : funcCSE(funcCSE){};
			FBody& operator=(const std::function<void()> fXXX){ new (&funcXXX) function<void()>(fXXX); return *this; }
			FBody& operator=(const std::function<void(EventType&)> fXXE){ new (&funcXXE) function<void(EventType&)>(fXXE); return *this; }
			FBody& operator=(const std::function<void(mui::Container*)> fXSX){ new (&funcXSX) function<void(mui::Container * owner)>(fXSX); return *this; }
			FBody& operator=(const std::function<void(mui::Container*, EventType&)> fXSE){ new (&funcXSE) function<void(mui::Container*, EventType&)>(fXSE); return *this; }
			FBody& operator=(const std::function<bool()> fCXX){ new (&funcCXX) function<bool()>(fCXX); return *this; }
			FBody& operator=(const std::function<bool(EventType&)> fCXE){ new (&funcCXE) function<bool(EventType&)>(fCXE); return *this; }
			FBody& operator=(const std::function<bool(mui::Container*)> fCSX){ new (&funcCSX) function<bool(mui::Container * owner)>(fCSX); return *this; }
			FBody& operator=(const std::function<bool(mui::Container*, EventType&)> fCSE){ new (&funcCSE) function<bool(mui::Container*, EventType&)>(fCSE); return *this; }
			std::function<void()> funcXXX;
			std::function<void(EventType&)> funcXXE;
			std::function<void(mui::Container*)> funcXSX;
			std::function<void(mui::Container*, EventType&)> funcXSE;
			std::function<bool()> funcCXX;
			std::function<bool(EventType&)> funcCXE;
			std::function<bool(mui::Container*)> funcCSX;
			std::function<bool(mui::Container*, EventType&)> funcCSE;
		};
		int listenerId;
		Container * listenerElement;
		FBody body;
		FType type;
	};
	
	template<typename EventType>
	class EventHandlerBase{
	public:
		EventHandlerBase(mui::Container * owner) : owner(owner){
		}
		
		// remove all containers belonging to listenerElement
		// returns the number of removed elements
		// pass listenerElement=nullptr to remove all unattached listeners
		int remove(mui::Container * listenerElement){
			int n = 0;
			for( auto it = listeners.begin(); it != listeners.end(); ){
				if((*it)->listenerElement == listenerElement){
					it = listeners.erase(it);
					n++;
				}
				else{
					++it;
				}
			}
			return n;
		}
		
		// remove all containers belonging with a listener id
		// returns the number of removed elements
		int remove(int listenerId){
			int n = 0;
			for( auto it = listeners.begin(); it != listeners.end(); ){
				if((*it)->listenerId == listenerId){
					it = listeners.erase(it);
					n++;
				}
				else{
					++it;
				}
			}
			return n;
		}
		
		/*
		// returns true if the event was consumed
		bool notify(EventType & event) {
			for (auto & listener : listeners) {
				bool res = listener.call(owner, event);
				if (listener.type >= 5 && res) return true;
			}

			return false;
		}*/
		
		// returns true if the event was consumed
		bool notify(EventType event){
			for( auto & listener : listeners ){
				bool res = listener.call(owner,event);
				if(listener.type >= 5 && res) return true;
			}
			
			return false;
		}

		
		
	protected:
		std::list<EventFunction<EventType>> listeners;
		mui::Container * owner;
	};
	
	class EventHandlerId{
	protected:
		static int nextListenerId;
		template<typename T>
		friend class EventHandlerBase;
		template<typename T>
		friend class EventHandler;
		template<typename T>
		friend class EventHandlerConsumable;
	};
	
	template<typename EventType>
	class EventHandler : public EventHandlerBase<EventType>{
	public:
		using EventHandlerBase<EventType>::EventHandlerBase;
		// adds a function and returns a code you can use to remove
		// your listener again
		int add(std::function<void()> func){
			this->listeners.emplace_back(EventHandlerId::nextListenerId,nullptr,func);
			return EventHandlerId::nextListenerId++;
		}
		int add(std::function<void(EventType &)> funcNoSender){
			this->listeners.emplace_back(EventHandlerId::nextListenerId,nullptr,funcNoSender);
			return EventHandlerId::nextListenerId++;
		}
		int add(std::function<void(mui::Container *, EventType &)> funcWithSender){
			this->listeners.emplace_back(EventHandlerId::nextListenerId,nullptr,funcWithSender);
			return EventHandlerId::nextListenerId++;
		}
		int add(mui::Container * listenerElement, std::function<void(EventType &)> funcNoSender ){
			this->listeners.emplace_back(EventHandlerId::nextListenerId,nullptr,funcNoSender);
			return EventHandlerId::nextListenerId++;
		}
		int add(mui::Container * listenerElement, std::function<void(mui::Container *, EventType &)> funcWithSender ){
			this->listeners.push_back(EventHandlerId::nextListenerId,listenerElement,funcWithSender);
			return EventHandlerId::nextListenerId++;
		}
	};
	
	template<typename EventType>
	class EventHandlerConsumable : public EventHandlerBase<EventType>{
	public:
		using EventHandlerBase<EventType>::EventHandlerBase;
		int add(std::function<bool()> func){
			this->listeners.emplace_back(EventHandlerId::nextListenerId,nullptr,func);
			return EventHandlerId::nextListenerId++;
		}
		int add(std::function<bool(EventType &)> funcNoSenderConsumable){
			this->listeners.emplace_back(EventHandlerId::nextListenerId,nullptr,funcNoSenderConsumable);
			return EventHandlerId::nextListenerId++;
		}
		int add(std::function<bool(mui::Container *, EventType &)> funcWithSenderConsumable){
			this->listeners.emplace_back(EventHandlerId::nextListenerId,nullptr,funcWithSenderConsumable);
			return EventHandlerId::nextListenerId++;
		}
		int add(mui::Container * listenerElement, std::function<bool(EventType &)> funcNoSenderConsumable ){
			this->listeners.emplace_back(EventHandlerId::nextListenerId,nullptr,funcNoSenderConsumable);
			return EventHandlerId::nextListenerId++;
		}
		int add(mui::Container * listenerElement, std::function<bool(mui::Container *, EventType &)> funcWithSenderConsumable ){
			this->listeners.emplace_back(EventHandlerId::nextListenerId,listenerElement,funcWithSenderConsumable);
			return EventHandlerId::nextListenerId++;
		}
	};
};

#endif /* MuiEvents_hpp */
