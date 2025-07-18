NAME		= PmergeMe
CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98
SRCDIR		= .
OBJDIR		= obj
INCDIR		= .

SRCS		= main.cpp \
			  PmergeMe.cpp

OBJS		= $(SRCS:%.cpp=$(OBJDIR)/%.o)

HEADERS		= PmergeMe.hpp

all: $(NAME)

$(NAME): $(OBJS)
	@echo "Linking $(NAME)..."
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(NAME) compiled successfully!"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) | $(OBJDIR)
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJDIR)

fclean: clean
	@echo "Cleaning executable..."
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
