
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include "ulliststr.h"

using namespace std;

ULListStr::ULListStr()
{
  head_ = NULL;
  tail_ = NULL;
  size_ = 0;
}

ULListStr::~ULListStr()
{
  clear();
}

bool ULListStr::empty() const
{
  return size_ == 0;
}

size_t ULListStr::size() const
{
  return size_;
}

// WRITE YOUR CODE HERE

void ULListStr::set(size_t loc, const string &val)
{
  std::string *ptr = getValAtLoc(loc);
  if (ptr == NULL)
  {
    throw std::invalid_argument("Bad location");
  }
  *ptr = val;
}

std::string &ULListStr::get(size_t loc)
{
  std::string *ptr = getValAtLoc(loc);
  if (ptr == NULL)
  {
    throw std::invalid_argument("Bad location");
  }
  return *ptr;
}

std::string const &ULListStr::get(size_t loc) const
{
  std::string *ptr = getValAtLoc(loc);
  if (ptr == NULL)
  {
    throw std::invalid_argument("Bad location");
  }
  return *ptr;
}

void ULListStr::clear()
{
  while (head_ != NULL)
  {
    Item *temp = head_->next;
    delete head_;
    head_ = temp;
  }
  tail_ = NULL;
  size_ = 0;
}

void ULListStr::push_back(const string &val)
{
  // Case #0 if array empyty
  if (tail_ == NULL)
  {
    Item *new_item = new Item;
    new_item->val[0] = val;
    new_item->next = NULL;
    new_item->prev = NULL;
    new_item->last++;
    new_item->first = 0; // place in FIRST position
    this->tail_ = new_item;
    this->head_ = new_item;
    this->size_++;
  }

  // Case #1 if array in tail node is full
  else if (this->tail_->last == ARRSIZE)
  {

    // 1. create new node with empty array and update val in first position
    Item *new_item = new Item;
    new_item->val[0] = val;

    // 2. place first/last in new node accordingly with val in FIRST position(for continuity)
    new_item->last = 1;
    new_item->first = 0;

    // 3. change tail pointer to point to new node
    new_item->prev = this->tail_;
    new_item->next = NULL;
    tail_->next = new_item;
    this->tail_ = new_item;
    this->size_++;

    // Case #2 if array in tail node is not full
  }
  else
  {
    this->tail_->val[this->tail_->last++] = val;
    this->size_++;
  }
}

void ULListStr::push_front(const string &val)
{
  // Case #0 if array empyty
  if (this->head_ == NULL)
  {
    Item *new_item = new Item;
    new_item->val[ARRSIZE - 1] = val;
    new_item->next = NULL;
    new_item->prev = NULL;
    new_item->first = ARRSIZE - 1; // place on LAST position
    new_item->last = ARRSIZE;

    this->head_ = new_item;
    this->tail_ = new_item;
    this->size_++;
  }

  // Case #1: (if full),
  else if (this->head_->first == 0)
  {
    // 1. create new node with empty array
    Item *new_item = new Item;
    // 2. place first/last in new node accordingly with val in LAST position(for continuity)
    new_item->val[ARRSIZE - 1] = val;
    new_item->first = ARRSIZE - 1;
    new_item->last = ARRSIZE;
    // 3. change head pointer to pointer to new node
    new_item->next = head_;
    this->head_->prev = new_item;
    this->head_ = new_item;
    this->size_++;
  }
  // Case #2: shift head first back by 1, add new val to the front
  else
  {
    this->head_->val[--head_->first] = val;
    this->size_++;
  }
}

void ULListStr::pop_back()
{

  // Case #1 if array in tail node
  if (this->tail_ == NULL)
  {
    return;
  }

  // Case #2  tail node array has more than one in contents
  else if (this->tail_->last - this->tail_->first > 1)
  {
    this->tail_->last--;
    this->size_--;
  }
  // Case #3 tail node array has one content and is same as head
  else if (this->tail_->last - this->tail_->first == 1 && this->head_ == this->tail_)
  {
    this->head_ = NULL;
    delete tail_;
    this->tail_ = NULL;
    this->size_--;
  }
  // Case #4 tail node array has one content and is  notsame as head
  else
  {
    Item *temp = tail_;
    this->tail_ = this->tail_->prev;
    this->tail_->next = NULL;
    delete temp;
    this->size_--;
  }
}

void ULListStr::pop_front()
{
  // Case #0 if array empyty
  if (this->head_ == NULL)
  {
    return;
  }

  // Case #1: node will be not  empty when popped
  else if (this->head_->last - this->head_->first > 1)
  {
    this->head_->first++;
    this->size_--;
  }

  // Case #2: head and tail are same node and they have 1 content
  else if (this->head_->last - this->head_->first == 1 && this->head_ == this->tail_)
  {
    this->head_ = NULL;
    delete this->tail_;
    this->tail_ = NULL;
    this->size_--;
  }
  // Case #4 head and tail are not the same node and head has one content
  else
  {
    Item *temp = this->head_;
    this->head_ = this->head_->next;
    this->head_->prev = NULL;
    delete temp;
    this->size_--;
  }
}

const string &ULListStr::front() const
{
  // head, first ptr
  size_t idx = this->head_->first;
  return this->head_->val[idx];
}

const string &ULListStr::back() const
{ // tail, last -1 ptr
  size_t idx2 = this->tail_->last - 1;
  return this->tail_->val[idx2];
}

string *ULListStr::getValAtLoc(size_t loc) const
{

  if (loc >= this->size_)
  {
    return NULL;
  }
  else
  {
    // start at head
    Item *current = this->head_;
    size_t sum = 0;
    size_t difference = current->last - current->first;
    // track of # of elements in each node until <= loc
    while (sum + difference <= loc)
    {
      sum += difference;
      current = current->next;
      difference = current->last - current->first;
    }
    // leftover(to find index of get val at current node)
    int adjust = loc - sum;
    int new_idx = current->first + adjust;
    return &current->val[new_idx];
  }
}
