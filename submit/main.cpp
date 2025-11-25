#include <iostream>
#include <stack>

template <class _Tp, std::size_t M> class Node;
template <class _Tp, std::size_t M> class BT;

/*
* 아래는 반드시 사용해야하는 Node 클래스입니다.
* 멤버 변수는 추가, 삭제 및 변경이 불가능합니다.
*/
template <class _Tp, std::size_t M = 4>
class Node {
	private: // Member types
		typedef _Tp					__key_type;
		typedef Node<__key_type, M>*	__node_pointer;
		typedef std::size_t			size_type;
	
	public: // Member variables
		__key_type		__keys_[M - 1];
		__node_pointer	__children_[M];
		size_type		__size_;
	
	public: // Constructor
		Node(): __size_(0) {
			for (size_type i = 0; i < M; i++) {
				__children_[i] = nullptr;
			}
		}
		Node(const __key_type& key): __size_(0) {
			for (size_type i = 0; i < M; i++) {
				__children_[i] = nullptr;
			}
			__push_front(key, nullptr);
		}

	public: // Modifier
		void __push_front(const __key_type& __key, const __node_pointer& __np) {
			__children_[__size_ + 1] = __children_[__size_];
			for (size_type __i = __size_; 0 < __i; --__i) {
				__keys_[__i] = __keys_[__i - 1];
				__children_[__i] = __children_[__i - 1];
			}
			__keys_[0] = __key;
			__children_[0] = __np;
			++size();
		}
		void __push_back(const __key_type& __key, const __node_pointer& __np) {
			__keys_[size()] = __key;
			__children_[size() + 1] = __np;
			++size();
		}
		__key_type __pop_front() {
			const __key_type __ret = __keys_[0];
			
			for (size_type __i = 0; __i < __size_ - 1; ++__i) {
				__keys_[__i] = __keys_[__i + 1];
				this->__children_[__i] = this->__children_[__i + 1];
			}
			this->__children_[__size_ - 1] = this->__children_[__size_];
			--size();

			return __ret;
		}
		__key_type __pop_back() { return __keys_[--size()]; }

	public: // Capacity
		size_type& size() { return __size_; }
		const size_type& size() const { return __size_; }

	friend std::ostream& operator<<(std::ostream& os, const __node_pointer& __np) {
		if (__np == nullptr || __np->size() == 0) {
			return os;
		}

		os << '<';
		os << __np->__children_[0] << ' ';
		for (std::size_t __i = 0; __i < __np->size() - 1; __i++) {
			os << __np->__keys_[__i] << ' ';
			if (__np->__children_[__i + 1] != nullptr) {
				os << __np->__children_[__i + 1] << ' ';
			}
		}
		os << __np->__keys_[__np->size() - 1] << ' ';
		os << __np->__children_[__np->size()];
		os << '>';

		return os;
	}
};


// 임시 노드. Node의 M에서 -1된 child들을 가진다.
template <class _Tp, std::size_t M = 5>
class TempNode {
	private: // Member types
		typedef _Tp					__key_type;
		typedef Node<__key_type, M-1>* __node_pointer;
		typedef std::size_t			size_type;
	
	public: // Member variables
		__key_type		__keys_[M - 1];
		__node_pointer	__children_[M];
		size_type		__size_;
	
	public: // Constructor
		TempNode(): __size_(0) {
			for (size_type i = 0; i < M; i++) {
				__children_[i] = nullptr;
			}
		}
		TempNode(const __key_type& key): __size_(0) {
			for (size_type i = 0; i < M; i++) {
				__children_[i] = nullptr;
			}
			__push_front(key, nullptr);
		}

	public: // Modifier
		void __push_front(const __key_type& __key, const __node_pointer& __np) {
			__children_[__size_ + 1] = __children_[__size_];
			for (size_type __i = __size_; 0 < __i; --__i) {
				__keys_[__i] = __keys_[__i - 1];
				__children_[__i] = __children_[__i - 1];
			}
			__keys_[0] = __key;
			__children_[0] = __np;
			++size();
		}
		void __push_back(const __key_type& __key, const __node_pointer& __np) {
			__keys_[size()] = __key;
			__children_[size() + 1] = __np;
			++size();
		}
		__key_type __pop_front() {
			const __key_type __ret = __keys_[0];
			
			for (size_type __i = 0; __i < __size_ - 1; ++__i) {
				__keys_[__i] = __keys_[__i + 1];
				this->__children_[__i] = this->__children_[__i + 1];
			}
			this->__children_[__size_ - 1] = this->__children_[__size_];
			--size();

			return __ret;
		}
		__key_type __pop_back() { return __keys_[--size()]; }

	public: // Capacity
		size_type& size() { return __size_; }
		const size_type& size() const { return __size_; }
};

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	if (__x == nullptr) {
		return;
	}
	
	for (std::size_t __i = 0; __i <= __x->size(); __i++) {
		__clear(__x->__children_[__i]);
	}
	delete __x;
	__x = nullptr;
}


// 아래는 반드시 사용해야하는 BT 클래스입니다.
template <class _Tp, std::size_t M = 4>
class BT {
	public: // Member types
		typedef _Tp						key_type;
		typedef std::size_t				size_type;
		typedef Node<key_type, M>*			pointer;
		typedef const Node<key_type, M>*	const_pointer;
	
	private: // Member variables
		pointer	__root_;
	
	public: // Constructor
		BT(): __root_(nullptr) {}
	
	public: // Lookup
		void inorder() const {
			// use __inorder or write your own code here
			__inorder(__root_);
			std::cout << std::endl;
		}
	private: // private functions

		template <class NodePtr>
		void insertKey(NodePtr x, pointer y, const key_type key)
		{
			int i = x->size() - 1;
			while(i >= 0 && key < x->__keys_[i])
			{
				// key들을 뒤로 민다.
				x->__keys_[i + 1] = x->__keys_[i];
				x->__children_[i + 2] = x->__children_[i + 1];
				i--;
			}
			x->__keys_[i + 1] = key;
			x->__children_[i + 2] = y;
			
			x->size()++;
		}
		// T에서 부터 찾아내려간다. stack pointer로 처리.
		std::pair<std::stack<pointer>, bool> searchPath(pointer T, const key_type key, std::stack<pointer>* stack)
		{
			std::stack<pointer> new_stack;
			// 만약 stack이 nullptr이라면 새로 만든다.
			if(stack == nullptr)
			{
				stack = &new_stack;
			}

			pointer __p = T;

			size_type i = 0;

			while(__p != nullptr)
			{
				i = 0;
				while(i < __p->size() && key > __p->__keys_[i])
				{
					i++;
				}

				stack->push(__p);

				// 만약 키를 찾은 상태라면 return.
				if(i < __p->size() && key == __p->__keys_[i])
				{
					return std::make_pair(*stack, true);
				}

				// children으로 이동.
				__p = __p->__children_[i];
			}

			return std::make_pair(*stack, false);
		}

		std::pair<pointer, key_type> splitNode(pointer &x, pointer y, key_type &key)
		{
			// tempNode를 M + 1로 만든다.
			TempNode<key_type, M+1>* tempNode = new TempNode<key_type, M+1>(key);

			size_type i = 0;
			//  __x의 내용을 전부 tempNode로 복사한다.
			for(i = 0; i < x->size(); i++)
			{
				tempNode->__keys_[i] = x->__keys_[i];
				tempNode->__children_[i] =  x->__children_[i];
			}
			// 마지막 children 복사.
			tempNode->__children_[i] = x->__children_[i];
			tempNode->size() = x->size();

			// 새로운 key를 넣기.
			insertKey(tempNode, y, key);

			// 중간 키를 제외하고 삽입, 중간키를 리턴해야 함.
			key_type centerKey = tempNode->__keys_[tempNode->size() / 2];

			x->size() = 0;
			i = 0;

			while(tempNode->__keys_[i] < centerKey)
			{
				x->__keys_[i] = tempNode->__keys_[i];
				x->__children_[i] = tempNode->__children_[i];
				i++;
				x->size()++;
			}
			// 여기도 마지막 children 복사.
			x->__children_[i] = tempNode->__children_[i];

			// 새 노드
			pointer newNode = new Node<key_type, M>();
			i++;
			// 노드 배열을 가져오기 위해 j 선언.
			size_type j = 0;

			while(i < tempNode->size())
			{
				newNode->__keys_[j] = tempNode->__keys_[i];
				newNode->__children_[j] = tempNode->__children_[i];
				i++;
				j++;
				newNode->size()++;
			}
			// 여기도 마지막 children 복사.
			newNode->__children_[j] = tempNode->__children_[i];

			delete tempNode;

			return std::make_pair(newNode, centerKey);
		}

		void deleteKey(pointer x, const key_type key)
		{
			size_type i = 0;
			while(key > x->__keys_[i])
			{
				i++;
			}

			while(i < x->size())
			{
				x->__keys_[i] = x->__keys_[i + 1];
				x->__children_[i + 1] = x->__children_[i + 2];
				i++;
			}
			x->size()--;
		}

		size_type bestSibiling(pointer x, pointer y)
		{
			size_type i = 0;
			while(y->__children_[i] != x)
			{
				i++;
			}

			if(i == 0)
			{
				return i + 1;
			}
			else if(i == y->size())
			{
				return i - 1;
			}
			// 왼쪽 자식이 만약 더 size가 크다면 왼쪽을 리턴, 아니면 오른쪽을 리턴.
			else
			{
				if(y->__children_[i]->size() >= y->__children_[i + 1]->size())
				{
					return i - 1;
				}
				else
				{
					return i + 1;
				}
			}
			
		}

		void redistributeKeys(pointer x, pointer y, size_type bestSib)
		{
			size_type i = 0;
			while(y->__children_[i] != x)
			{
				i++;
			}

			pointer bestNode = y->__children_[bestSib];

			// 만약 best가 왼쪽이라면
			if(bestSib < i)
			{
				// bestnode의 마지막 key값. x에 있는 키들보다 작다.
				key_type lastKey = bestNode->__keys_[bestNode->size() - 1];
				// x에 y의 키(i - 1)을 추가한다. (x의 키들보다 작기에 첫번째 값으로 들어가게 된다.)
				insertKey(x, nullptr, y->__keys_[i - 1]);
				x->__children_[1] = x->__children_[0];
				// children을 bestNode의 lastKey의 오른쪽 children으로 만들어 준다. 
				// 여전히 last key보다 작은 children이기에 왼쪽에 둔다.
				x->__children_[0] = bestNode->__children_[bestNode->size()];
				bestNode->__children_[bestNode->size()] = nullptr;
				// 해당 마지막 키를 bestNode에서 y로 옮긴다.
				deleteKey(bestNode, lastKey);
				y->__keys_[i - 1] = lastKey;
			}
			else
			{
				// 위와 반대로 오른쪽에 있는 경우에는 y의 오른쪽 키를 x에 내려주고,
				// best sibiling의 첫번째 (x보다 항상 큼)을 x의 부모 키로 설정해준다.
				key_type firstKey = bestNode->__keys_[0];
				insertKey(x, nullptr, y->__keys_[i]);
				x->__children_[x->size()] = bestNode->__children_[0];
				bestNode->__children_[0] = bestNode->__children_[1];
				deleteKey(bestNode, firstKey);
				y->__keys_[i] = firstKey;
			}
		}

		void mergeNode(pointer &x, pointer y, size_type bestSib)
		{
			size_type i = 0;
			while(y->__children_[i] != x)
			{
				i++;
			}

			pointer bestNode = y->__children_[bestSib];
			// 만약 bestsibiling이 오른쪽 이라면, x와 바꾼다.(항상 bestsib를 왼쪽으로 만들기 위함.)
			if(bestSib > i)
			{
				size_type temp = i; i = bestSib; bestSib = temp;
				pointer tmpnode = x; x = bestNode; bestNode = tmpnode;
			}
			bestNode->__keys_[bestNode->size()] = y->__keys_[i - 1];
			bestNode->size()++;

			// x를 bestnode에 추가.
			size_type j = 0;
			while(j < x->size())
			{
				bestNode->__keys_[bestNode->size()] = x->__keys_[j];
				bestNode->__children_[bestNode->size()] = x->__children_[j];
				bestNode->size()++;
				j++;
			}
			bestNode->__children_[bestNode->size()] = x->__children_[x->size()];

			// y의 키를 삭제한다.
			deleteKey(y, y->__keys_[i - 1]);
			// x를 할당 해제.
			delete x;
		}

	public: // Modifier
		std::pair<pointer, bool> insert(const key_type& key) {
			if (__root_ == nullptr) {
				__root_ = new Node<key_type, M>(key);
				return std::make_pair(__root_, true);
			}

			std::pair<std::stack<pointer>, bool> path_result = searchPath(__root_, key, nullptr);

			std::stack<pointer> stack = path_result.first;
			bool found = path_result.second;
			if(found)
			{
				return std::make_pair(nullptr, false);
			}

			bool finished = false;
			pointer __p = stack.top();
			stack.pop();
			pointer __q = nullptr;

			key_type newKey = key;

			while(!finished)
			{
				if(__p->size() < M - 1)
				{
					// insert key.
					insertKey(__p, __q, newKey);
					finished = true;
				}
				// overflow 발생.
				else
				{
					std::pair<pointer, _Tp> splitResult = splitNode(__p, __q, newKey);
					pointer newNode = splitResult.first;
					newKey = splitResult.second;
					__q = newNode;
					if(!stack.empty())
					{
						__p = stack.top();
						stack.pop();
					}
					else
					{
						__root_ = new Node<key_type, M>();
						__root_->__keys_[0] = newKey;
						__root_->__children_[0] = __p;
						__root_->__children_[1] = __q;
						__root_->size() = 1;
						finished = true;
					}
				}
			}

			return std::make_pair(__p, true);
		}

		const_pointer erase(const key_type& key) {
			std::pair<std::stack<pointer>, bool> path_result = searchPath(__root_, key, nullptr);
			std::stack<pointer> stack = path_result.first;
			bool found = path_result.second;
			
			// 만약 키를 못찾으면 nullptr 리턴.
			if(!found)
			{
				return nullptr;
			}

			// p: 해당 키가 있는 노드.
			pointer p = stack.top();
			stack.pop();
			pointer q = nullptr;

			// key가 내부 노드에 있는 경우 단말노드의 키와 교체하여 단말노드로부터 삭제를 진행해야 한다.
			if(p->__children_[0] != nullptr)
			{
				pointer internalNode = p;
				size_type i = 0;
				// 키가 있는 위치를 찾는다.
				while(i < p->size())
				{
					if(p->__keys_[i] == key)
						break;
					i++;
				}

				stack.push(p);
				searchPath(p->__children_[i + 1], p->__keys_[i], &stack);

				// 현재 노드의 child에서 찾았으니, key값 다음으로 큰값을 찾게 되고, 그 값을 key값으로 교체하게 된다.
				p = stack.top();
				stack.pop();
				key_type temp = internalNode->__keys_[i];
				internalNode->__keys_[i] = p->__keys_[0];
				p->__keys_[0] = temp;
			}

			bool finished = false;
			deleteKey(p, key);

			if(!stack.empty())
			{
				q = stack.top();
				stack.pop();
			}

			while(!finished)
			{
				if(__root_ == p || p->size() >= (M - 1) / 2)
				{
					finished = true;
				}
				else
				{
					size_type bestSib = bestSibiling(p, q);

					// size가 커서 merge하면 불리할 때 redistribute를 해야 한다.
					if(q->__children_[bestSib]->size() > (M - 1) / 2)
					{
						redistributeKeys(p, q, bestSib);
						finished = true;
					}
					// 그게 아니라면 merge를 진행한다. merge를 한 후 위로 계속 올라가며 균형을 맞춰야 한다.
					else
					{
						mergeNode(p, q, bestSib);
						p = q;

						if(!stack.empty())
						{
							q = stack.top();
							stack.pop();
						}
						else
						{
							finished = true;
						}
					}

				}
			}

			if(q != nullptr && q->size() == 0)
			{
				__root_ = q->__children_[0];
				delete q;
			}

			return p;
		}

		void clear() {
			// use __clear or write your own code here
			__clear(__root_);
		}
	
	/*
	* 아래는 inorder traversal을 대체할 수 있는 operator<< 입니다.
	* 반드시 아래의 함수를 사용해야할 필요는 없습니다.
	*/
	friend std::ostream& operator<<(std::ostream& os, const BT& tree) {
		os << tree.__root_;
		return os;
	}
};

/*
* 아래는 추천드리는 main 함수의 예시입니다.
* 반드시 아래의 main 함수를 사용해야할 필요는 없습니다.
* ❗️새로 구현하실 경우, 출력 형식에 주의하세요.❗️
*/
int main() {
	BT<int, 6>	tree;
	char	command;
	int		key;

	while (std::cin >> command >> key) {
		switch ((int)command) {
			case (int)'i':
				if (tree.insert(key).second == false) {
					std::cerr << "i " << key << ": The key already exists" << std::endl;
					continue;
				}
				break;
			case (int)'d':
				if (tree.erase(key) == nullptr) {
					std::cerr << "d " << key << ": The key does not exist" << std::endl;
					continue;
				}
				break;
			default:
				std::cerr << "Invalid command: " << command << std::endl;
				return (1);
				break;
		}
		std::cout << tree << std::endl;
	}

	// 프로그램 종료 전, 메모리 누수가 발생하지 않도록 할당받은 메모리를 반드시 해제해야 합니다.
	tree.clear();

	return (0);
}
