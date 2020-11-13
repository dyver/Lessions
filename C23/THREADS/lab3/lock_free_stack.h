#pragma once
#include <memory>
#include <atomic>

// *********** TASK 2 *****************
template <typename T>
class lock_free_stack{
private:
	struct Node {
		Node *next;
		std::shared_ptr<T> data;

		Node(const T &data_):data(std::make_shared<T>(data_)){}
	};

	std::atomic<Node*> head;

public:
	void push(const T& data) {
		const Node *new_Node = new Node(data);
		new_Node->next = head.load();
		while (!head.compare_exchange_weak(new_Node->next, new_Node));
	}

	std::shared_ptr<T> pop() {
		Node* old_head = head.load();
		while (old_head &&
			!head.compare_exchange_weak(old_head, old_head->next));
		return old_head ? old_head->data : std::shared_ptr<T>();
	}
};

// *********** TASK 3 *****************
template <typename T>
class lf_stack_enhanced{
private:
	struct Node {
		Node *next;
		std::shared_ptr<T> data;

		Node(const T &data_) :data(std::make_shared<T>(data_)) {}
	};

	std::atomic<Node*> head;
	std::atomic<Node*> to_be_deleted;
	std::atomic<unsigned> threads_in_pop;

	void try_reclaim(Node* old_head) {
		if (threads_in_pop == 1) {
			Node *nodes_to_delete = to_be_deleted.exchange(nullptr);
			if (!--threads_in_pop) {
				delete_nodes(nodes_to_delete);
			}
			else if(nodes_to_delete) {
				chain_pending_nodes(nodes_to_delete);
			}
			delete old_head;
		}
		else {
			chain_pending_node(old_head);
			--threads_in_pop;
		}
	}

	void chain_pending_nodes(Node* nodes) {
		Node *last = nodes;
		while (const Node *next = last->next) {
			last = next;
		}
		chain_pending_nodes(nodes, last);
	}

	void chain_pending_nodes(Node *first, Node *last) {
		last->next = to_be_deleted;
		while ( !to_be_deleted.compare_exchange_weak( last->next, first ) );
	}

	void chain_pending_node(Node *n) {
		chain_pending_nodes(n, n);
	}

	static void delete_nodes(Node *nodes) {
		while (nodes) {
			Node *next = nodes->next;
			delete nodes;
			nodes = next;
		}
	}

public:
	std::shared_ptr<T> pop() {
		++threads_in_pop;
		Node* old_head = head.load();
		while ( old_head && !head.compare_exchange_weak( old_head, old_head->next ) );
		std::shared_ptr<T> result;
		if (old_head) {
			result.swap(old_head->data);
		}
		try_reclaim(old_head);
		return result;
	}

	void push(const T& data) {
		const Node *new_Node = new Node(data);
		new_Node->next = head.load();
		while (!head.compare_exchange_weak(new_Node->next, new_Node));
	}
};