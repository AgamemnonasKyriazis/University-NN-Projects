y = load("-ascii", "test.txt");
k = load("-ascii", "test2.txt");

y = transpose(y);
k = transpose(k);

K = 13;
figure(1);
hold on;
for i = [1:1:K+1]
  scatter(y(1, :)(y(3, :) == i), y(2, :)(y(3, :) == i), "+")
endfor;
scatter(k(1, :), k(2, :), "k", "*")
hold off;