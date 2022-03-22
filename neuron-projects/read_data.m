y_hat = load("-ascii", "test.txt");
y = load("-ascii", "test_data.txt");

y_hat = transpose(y_hat);
y = transpose(y);
e = y_hat(3, :).-y(3, :);
columns(e(e == 0))/columns(e)

figure(1);
hold on;
for i = [1:1:4+1]
  scatter(y_hat(1, :)(y_hat(3, :) == i), y_hat(2, :)(y_hat(3, :) == i), "+")
endfor;
hold off;