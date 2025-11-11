#!/bin/sh

echo "🔨 1) make re ..."
make re || { echo "❌ make re failed"; exit 1; }
echo "✅ make re done"

echo
echo "🧪 2) valgrind helper: echo hi ..."
if [ -x ./scripts/leak-check.sh ]; then
  printf "echo hi\nexit\n" | ./scripts/leak-check.sh
else
  echo "⚠️ ./scripts/leak-check.sh مش موجود أو مش executable، هنتخطاه."
fi

echo
echo "🧪 3) valgrind helper: /bin/ls ..."
if [ -x ./scripts/leak-check.sh ]; then
  printf "/bin/ls\nexit\n" | ./scripts/leak-check.sh
fi

echo
echo "📏 4) norminette src include ..."
if command -v norminette >/dev/null 2>&1; then
  norminette src include
else
  echo "⚠️ norminette مش متسطبة على الماك، عادي."
fi

echo
echo "🎉 done."
